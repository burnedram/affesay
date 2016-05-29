// This header contains a copy of the the CPlayerInfo class
// from hl2sdk-csgo/game/server/player.h
// However the original class does not contain GetAssistsCount(),
// nor does IPlayerInfo, which has been added here.
//
// The order of the functions is very important, as the vtable
// used will be the one from csgo/bin/server.so.
// Therefore the function positions in the vtable must be the same
//
// I.e. if you switch GetFragCount()'s and GetAssistsCount()'s places
// they will return each other's count.
//
// The fact that GetLastUserCommand() seems to work also hints that
// all other functions should work, but I have not tested
// to call them all. All Get() and Is() works however. 
//
// Usage: CPlayerInfo *pInfo = (CPlayerInfo *)pIPlayerInfo;
#include <iplayerinfo.h>

class CBasePlayer;
class CPlayerInfo : public IBotController, public IPlayerInfo {
    public:
        CPlayerInfo () { m_pParent = NULL; } 
        ~CPlayerInfo () {}
        void SetParent( CBasePlayer *parent ) { m_pParent = parent; } 

        // IPlayerInfo interface
        virtual const char *GetName();
        virtual int         GetUserID();
        virtual const char *GetNetworkIDString();
        virtual int         GetTeamIndex();
        virtual void        ChangeTeam( int iTeamNum );
        virtual int         GetFragCount();
        // This function is missing from hl2sdk-csgo, it's even missing from the entire internet.
        // Try googling "GetAssistsCount", none of the results reference csgo source code.
        virtual int         GetAssistsCount();
        virtual int         GetDeathCount();
        virtual bool        IsConnected();
        virtual int         GetArmorValue();

        virtual bool IsHLTV();
#if defined( REPLAY_ENABLED )
        virtual bool IsReplay();
#endif
        virtual bool IsPlayer();
        virtual bool IsFakeClient();
        virtual bool IsDead();
        virtual bool IsInAVehicle();
        virtual bool IsObserver();
        virtual const Vector GetAbsOrigin();
        virtual const QAngle GetAbsAngles();
        virtual const Vector GetPlayerMins();
        virtual const Vector GetPlayerMaxs();
        virtual const char *GetWeaponName();
        virtual const char *GetModelName();
        virtual const int GetHealth();
        virtual const int GetMaxHealth();

        // bot specific functions   
        virtual void SetAbsOrigin( Vector & vec );
        virtual void SetAbsAngles( QAngle & ang );
        virtual void RemoveAllItems( bool removeSuit );
        virtual void SetActiveWeapon( const char *WeaponName );
        virtual void SetLocalOrigin( const Vector& origin );
        virtual const Vector GetLocalOrigin( void );
        virtual void SetLocalAngles( const QAngle& angles );
        virtual const QAngle GetLocalAngles( void );
        virtual void PostClientMessagesSent( void );
        virtual bool IsEFlagSet( int nEFlagMask );

        virtual void RunPlayerMove( CBotCmd *ucmd );
        virtual void SetLastUserCommand( const CBotCmd &cmd );

        virtual CBotCmd GetLastUserCommand();

    private:
        CBasePlayer *m_pParent; 
};
