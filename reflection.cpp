#include "reflection.h"
#include "ccsplayer.h"
#include "affesayplugin.h"

#include <cxxabi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <link.h>
#include <elf.h>

struct CallbackStruct {
    const char *pszName;
    ElfW(Addr)  uiBaseAddress;
};

static int callback(struct dl_phdr_info *info, size_t size, void *data) {
    if(strcmp(static_cast<CallbackStruct *>(data)->pszName, info->dlpi_name))
        return 0;
    static_cast<CallbackStruct *>(data)->uiBaseAddress = info->dlpi_addr;
    return 1;
}

const ElfW(Addr) queryLibraryBase(const char *pszLibraryName) {
    struct CallbackStruct data = {};
    data.pszName = pszLibraryName;
    dl_iterate_phdr(callback, static_cast<void *>(&data));
    return data.uiBaseAddress;
}


bool initReflection() {
    INFO("Initializing reflection...");
    // get base address of server.so
    char *pszLib = static_cast<char *>(malloc(PATH_MAX));
    getcwd(pszLib, PATH_MAX);
    strncat(pszLib, "/csgo/bin/server.so", PATH_MAX - strlen(pszLib));
    const ElfW(Addr) uiBaseAddress = queryLibraryBase(pszLib);
    DEBUG("uiBaseAddress %#010x", uiBaseAddress);
    if(!uiBaseAddress) {
        free(pszLib);
        ERROR("Unable to get base address of server.so, reflection disabled");
        return false;
    }
    
    FILE *pFile = fopen(pszLib, "r");
    free(pszLib);
    if(!pFile) {
        ERROR("Unable to open server.so for reading, reflection disabled");
        return false;
    }
    ElfW(Ehdr) elfHeader;
    fread(&elfHeader, 1, sizeof(ElfW(Ehdr)), pFile);
    if(elfHeader.e_shoff == SHN_UNDEF) {
        ERROR("server.so does not contain a section header table, reflection disabled");
        fclose(pFile);
        return false;
    }
    if(elfHeader.e_shstrndx == SHN_XINDEX) {
        ERROR("e_shstrndx == SHN_XINDEX not implemented, reflection disabled");
        fclose(pFile);
        return false;
    }

    // find and read name table of sections
    fseek(pFile, elfHeader.e_shoff + elfHeader.e_shstrndx * sizeof(ElfW(Shdr)), SEEK_SET);
    ElfW(Shdr) sectionHeader;
    fread(&sectionHeader, 1, sizeof(ElfW(Shdr)), pFile);
    if(sectionHeader.sh_type == SHT_NOBITS) {
        ERROR("Table of section names is of type SHT_NOBITS, reflection disabled");
        fclose(pFile);
        return false;
    }
    char *sectionNames = static_cast<char *>(malloc(sectionHeader.sh_size));
    fseek(pFile, sectionHeader.sh_offset, SEEK_SET);
    fread(sectionNames, 1, sectionHeader.sh_size, pFile);

    // find and read symbol table header and string table header
    // there are multiple string tables, but we want the one named ".strtab"
    bool bSymtabHeader = false;
    ElfW(Shdr) symtabHeader;
    bool bStrtabHeader = false;
    ElfW(Shdr) strtabHeader;
    fseek(pFile, elfHeader.e_shoff, SEEK_SET);
    for(ElfW(Half) i = 0; i < elfHeader.e_shnum; i++) {
        fread(&sectionHeader, 1, sizeof(ElfW(Shdr)), pFile);
        if(sectionHeader.sh_type == SHT_NULL)
            continue; // entry not used
        const char *pszName = NULL;
        if(sectionHeader.sh_name)
            pszName = sectionNames + sectionHeader.sh_name;
        if(sectionHeader.sh_type == SHT_SYMTAB) {
            if(bSymtabHeader) {
                ERROR("Multiple symbol tables, reflection disabled");
                fclose(pFile);
                free(sectionNames);
                return false;
            }
            memcpy(&symtabHeader, &sectionHeader, sizeof(ElfW(Shdr)));
            bSymtabHeader = true;
        } else if(sectionHeader.sh_type == SHT_STRTAB && !strcmp(pszName, ".strtab")) {
            if(bStrtabHeader) {
                ERROR("Multiple string tables, reflection disabled");
                fclose(pFile);
                free(sectionNames);
                return false;
            }
            memcpy(&strtabHeader, &sectionHeader, sizeof(ElfW(Shdr)));
            bStrtabHeader = true;
        }
    }

    if(!bSymtabHeader) {
        ERROR("No symbol table found in server.so, reflection disabled");
        fclose(pFile);
        free(sectionNames);
        return false;
    }
    if(!bStrtabHeader) {
        ERROR("No string table found in server.so, reflection disabled");
        fclose(pFile);
        free(sectionNames);
        return false;
    }
    if(symtabHeader.sh_type == SHT_NOBITS) {
        ERROR("Symbol table is of type SHT_NOBITS, reflection disabled");
        fclose(pFile);
        free(sectionNames);
        return false;
    }
    if(strtabHeader.sh_type == SHT_NOBITS) {
        ERROR("String table is of type SHT_NOBITS, reflection disabled");
        fclose(pFile);
        free(sectionNames);
        return false;
    }
    ElfW(Word) uiSymbols = symtabHeader.sh_size / symtabHeader.sh_entsize;
    DEBUG("Found %d symbols in server.so", uiSymbols);

    // read string table
    char *stringTable = static_cast<char *>(malloc(strtabHeader.sh_size));
    fseek(pFile, strtabHeader.sh_offset, SEEK_SET);
    fread(stringTable, 1, strtabHeader.sh_size, pFile);

    // iterate over all symbols in symbol table
    ElfW(Sym) symbol;
    fseek(pFile, symtabHeader.sh_offset, SEEK_SET);
    int status;
    char *mangled, *demangled;
    for(ElfW(Word) i = 0; i < uiSymbols; i++) {
        fread(&symbol, 1, symtabHeader.sh_entsize, pFile);
        mangled = stringTable + symbol.st_name;
        demangled = abi::__cxa_demangle(mangled, 0, 0, &status);
        if(status) {
            //WARN("Unable to demangle %s, reason %d", mangled, status);
        } else {
            if(!REFLECTION_CMP_V(CCSPlayer, GetAccountBalance, demangled)) {
                REFLECTION_ASSIGN_I_V(CCSPlayer, GetAccountBalance, uiBaseAddress + symbol.st_value);
            } else if(!REFLECTION_CMP_I(CCSPlayer, SetHealth, demangled)) {
                REFLECTION_ASSIGN_V_I(CCSPlayer, SetHealth, uiBaseAddress + symbol.st_value);
            } else if(!REFLECTION_CMP_FFF(CCSPlayer, Blind, demangled)) {
                REFLECTION_ASSIGN_V_FFF(CCSPlayer, Blind, uiBaseAddress + symbol.st_value);
            } else if(!REFLECTION_CMP_I(CCSPlayer, Instance, demangled)) {
                REFLECTION_ASSIGN_Z_I(CCSPlayer, Instance, CCSPlayer *, uiBaseAddress + symbol.st_value);
            }
            free(demangled);
        }
    }
    free(sectionNames);
    free(stringTable);

    bool missing = false;
    if(!REFLECTION_I_V(CCSPlayer, GetAccountBalance)) {
        ERROR(REFLECTION_DEMANGLE_V(CCSPlayer, GetAccountBalance) " not found, reflection disabled");
        missing = true;
    }
    if(!REFLECTION_V_I(CCSPlayer, SetHealth)) {
        ERROR(REFLECTION_DEMANGLE_I(CCSPlayer, SetHealth) " not found, reflection disabled");
        missing = true;
    }
    if(!REFLECTION_V_FFF(CCSPlayer, Blind)) {
        ERROR(REFLECTION_DEMANGLE_FFF(CCSPlayer, Blind) " not found, reflection disabled");
        missing = true;
    }
    if(!REFLECTION_Z_I(CCSPlayer, Instance)) {
        ERROR(REFLECTION_DEMANGLE_I(CCSPlayer, Instance) " not found, reflection disabled");
        missing = true;
    }
    if(missing)
        return false;
    INFO("Reflection initialized");
    return true;
}
