#include <iserverplugin.h>
#include <eiface.h>
#include <iplayerinfo.h>
#include <tier1.h>
#include <convar.h>
#include "recipientfilters.h"
#include "cstrike15_usermessages.pb.h"

ConVar myVar("myVar", "42", FCVAR_REPLICATED | FCVAR_NOTIFY, "A number...");
void MyFun() {
    Msg("HELLO FROM THE OUTSIIIIIIIDEEEEEEEE\n");
}
ConCommand myComm("myComm", MyFun, "Theo pizza pls", 0);
class ServerPluginCallbacks : public IServerPluginCallbacks
{
    private:
        IVEngineServer *engine = NULL;
        ICvar *icvar = NULL;
        IPlayerInfoManager *playerinfomanager = NULL;
        CGlobalVars *globals = NULL;
    public:
	// Initialize the plugin to run
	// Return false if there is an error during startup.
    virtual bool Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory)
    {
        ConnectTier1Libraries(&interfaceFactory, 1);
        engine = (IVEngineServer *) interfaceFactory(INTERFACEVERSION_VENGINESERVER, NULL);
        if(!engine)
            ConMsg("No engine\n");
        icvar = (ICvar *) interfaceFactory(CVAR_INTERFACE_VERSION, NULL);
        if(!icvar)
            ConMsg("No icvar\n");
        icvar->RegisterConCommand(&myVar);
        icvar->RegisterConCommand(&myComm);
        playerinfomanager = (IPlayerInfoManager *) gameServerFactory(INTERFACEVERSION_PLAYERINFOMANAGER, NULL);
        if(!playerinfomanager)
            ConMsg("No playerinfomanager\n");
        globals = playerinfomanager->GetGlobalVars();
        return true;
    }

	// Called when the plugin should be shutdown
	virtual void			Unload( void )
    {
    }

	// called when a plugins execution is stopped but the plugin is not unloaded
	virtual void			Pause( void )
    {
    }

	// called when a plugin should start executing again (sometime after a Pause() call)
	virtual void			UnPause( void )
    {
    }

	// Returns string describing current plugin.  e.g., Admin-Mod.  
	virtual const char     *GetPluginDescription( void )
    {
        return "MrAffe";
    }

	// Called any time a new level is started (after GameInit() also on level transitions within a game)
	virtual void			LevelInit( char const *pMapName )
    {
    }

	// The server is about to activate
	virtual void			ServerActivate( edict_t *pEdictList, int edictCount, int clientMax )
    {
    }

	// The server should run physics/think on all edicts
	virtual void			GameFrame( bool simulating )
    {
    }

	// Called when a level is shutdown (including changing levels)
	virtual void			LevelShutdown( void )
    {
    }

	// Client is going active
	virtual void			ClientActive( edict_t *pEntity )
    {
    }

	// Client is fully connected ( has received initial baseline of entities )
	virtual void			ClientFullyConnect( edict_t *pEntity )
    {
    }

	// Client is disconnecting from server
	virtual void			ClientDisconnect( edict_t *pEntity )
    {
    }
	
	// Client is connected and should be put in the game
	virtual void			ClientPutInServer( edict_t *pEntity, char const *playername )
    {
    }

	// Sets the client index for the client who typed the command into their console
	virtual void			SetCommandClient( int index )
    {
    }

	// A player changed one/several replicated cvars (name etc)
	virtual void			ClientSettingsChanged( edict_t *pEdict )
    {
    }

	// Client is connecting to server ( set retVal to false to reject the connection )
	//	You can specify a rejection message by writing it into reject
	virtual PLUGIN_RESULT	ClientConnect( bool *bAllowConnect, edict_t *pEntity, const char *pszName, const char *pszAddress, char *reject, int maxrejectlen )
    {
        return PLUGIN_CONTINUE;
    }

	// The client has typed a command at the console
	virtual PLUGIN_RESULT	ClientCommand( edict_t *pEdict, const CCommand &args )
    {
        int entindex = pEdict - globals->pEdicts;
        CCSUsrMsg_SayText msg;
        ChatFilter filter(entindex);

        ConMsg("COMMAND: %s///\n", args.GetCommandString());
        if(!strcasecmp("affe ", args.GetCommandString())) {
                ConMsg("SENDING STUFF\n");
                msg.set_text("t\x01je\x02n\x03na");
                msg.set_chat(true);
                engine->SendUserMessage(filter, CS_UM_SayText, msg);
        }
        engine->ClientPrintf(pEdict, "T\x01j\x02e\x03n\x03n\x04a\a");
        return PLUGIN_CONTINUE;
    }

	// A user has had their network id setup and validated 
	virtual PLUGIN_RESULT	NetworkIDValidated( const char *pszUserName, const char *pszNetworkID )
    {
        return PLUGIN_CONTINUE;
    }

	// This is called when a query from IServerPluginHelpers::StartQueryCvarValue is finished.
	// iCookie is the value returned by IServerPluginHelpers::StartQueryCvarValue.
	// Added with version 2 of the interface.
	virtual void			OnQueryCvarValueFinished( QueryCvarCookie_t iCookie, edict_t *pPlayerEntity, EQueryCvarValueStatus eStatus, const char *pCvarName, const char *pCvarValue )
    {
    }

	// added with version 3 of the interface.
	virtual void			OnEdictAllocated( edict_t *edict )
    {
    }
	virtual void			OnEdictFreed( const edict_t *edict  )
    {
    }	
};

ServerPluginCallbacks g_Plugin;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(ServerPluginCallbacks, IServerPluginCallbacks, INTERFACEVERSION_ISERVERPLUGINCALLBACKS, g_Plugin );
