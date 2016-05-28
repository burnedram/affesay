#ifndef AFFESAYPLUGIN_H
#define AFFESAYPLUGIN_H

#include <iserverplugin.h>
#include <eiface.h>
#include <iplayerinfo.h>
#include <tier1.h>

#define ENTINDEX(pEdict) pEdict - pGlobals->pEdicts
#define LOG(level, format, ...) ConMsg("%s: [" level "] " format"\n", g_Plugin.GetPluginDescription(), ##__VA_ARGS__)
#define INFO( format, ...) LOG("INFO",  format, ##__VA_ARGS__)
#define DEBUG(format, ...) LOG("DEBUG", format, ##__VA_ARGS__)
#define WARN( format, ...) LOG("WARN",  format, ##__VA_ARGS__)
#define ERROR(format, ...) LOG("ERROR", format, ##__VA_ARGS__)

class AffeSayPlugin : public IServerPluginCallbacks {
    public:
                                AffeSayPlugin();
        virtual                 ~AffeSayPlugin();

        // IServerPluginCallbacks methods
        virtual bool            Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory );
        virtual void            Unload();
        virtual void            Pause();
        virtual void            UnPause();
        virtual const char     *GetPluginDescription();      
        virtual void            LevelInit(char const *pszMapName);
        virtual void            ServerActivate(edict_t *pEdictList, int edictCount, int clientMax);
        virtual void            GameFrame(bool simulating);
        virtual void            LevelShutdown();
        virtual void            ClientActive(edict_t *pEntity);
        virtual void            ClientFullyConnect(edict_t *pEntity);
        virtual void            ClientDisconnect(edict_t *pEntity);
        virtual void            ClientPutInServer(edict_t *pEntity, char const *pszPlayername);
        virtual void            SetCommandClient(int index);
        virtual void            ClientSettingsChanged(edict_t *pEdict);
        virtual PLUGIN_RESULT   ClientConnect(bool *bAllowConnect, edict_t *pEntity, const char *pszName, const char *pszAddress, char *pszReject, int maxrejectlen);
        virtual PLUGIN_RESULT   ClientCommand(edict_t *pEntity, const CCommand &args);
        virtual PLUGIN_RESULT   NetworkIDValidated(const char *pszUserName, const char *pszNetworkID);
        virtual void            OnQueryCvarValueFinished(QueryCvarCookie_t iCookie, edict_t *pPlayerEntity, EQueryCvarValueStatus eStatus, const char *pszCvarName, const char *pszCvarValue);
        virtual void            OnEdictAllocated(edict_t *pEdict);
        virtual void            OnEdictFreed(const edict_t *pEdict);  
};

extern AffeSayPlugin g_Plugin;
// We get g_pCVar from tier1.h
//extern ICvar *g_pCVar;
extern IVEngineServer *g_pEngine;
extern IPlayerInfoManager *g_pPlayerInfoManager;
extern CGlobalVars *g_pGlobals;
extern int g_iMaxPlayers;

extern void RegisterConCommands();

#endif
