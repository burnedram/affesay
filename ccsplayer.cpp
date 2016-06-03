#include "ccsplayer.h"
#include "reflection.h"

#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wuninitialized"
REFLECTION_DECLARE_Z_I(CCSPlayer, Instance, CCSPlayer *);
REFLECT_Z_I(CCSPlayer, Instance, CCSPlayer *);

REFLECTION_DECLARE_I_V(CCSPlayer, GetAccountBalance);
REFLECT_I_V(CCSPlayer, GetAccountBalance);

REFLECTION_DECLARE_V_I(CCSPlayer, SetHealth);
REFLECT_V_I(CCSPlayer, SetHealth);

REFLECTION_DECLARE_V_FFF(CCSPlayer, Blind);
REFLECT_V_FFF(CCSPlayer, Blind);

const char *CCSPlayer::GetClanName() {
    const char *pszName;
    asm ("movl %eax,-0x4(%ebp)");    // %eax is this-pointer
    asm ("addl $0x2188,-0x4(%ebp)"); // -0x4(%ebp) is pszName
    return pszName;
};
#pragma GCC diagnostic error "-Wreturn-type"
#pragma GCC diagnostic error "-Wuninitialized"
