#ifndef CCSPLAYER_H
#define CCSPLAYER_H

#include "reflection.h"

class CCSPlayer;

extern REFLECTION_DECLARE_Z_I(CCSPlayer, Instance, CCSPlayer *);
extern REFLECTION_DECLARE_I_V(CCSPlayer, GetAccountBalance);
extern REFLECTION_DECLARE_V_I(CCSPlayer, SetHealth);
extern REFLECTION_DECLARE_V_FFF(CCSPlayer, Blind);

class CCSPlayer {
    public:
        static CCSPlayer *Instance(int);
        int GetAccountBalance();
        void SetHealth(int);
        void Blind(float, float ,float);
        const char *GetClanName();
};

#endif
