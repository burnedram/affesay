#include "affesayplugin.h"

AffeSayPlugin g_Plugin;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(AffeSayPlugin, IServerPluginCallbacks, INTERFACEVERSION_ISERVERPLUGINCALLBACKS, g_Plugin);

IVEngineServer *g_pEngine = NULL;
IPlayerInfoManager *g_pPlayerInfoManager = NULL;
CGlobalVars *g_pGlobals = NULL;
int g_iMaxPlayers = 0;

AffeSayPlugin::AffeSayPlugin() {}
AffeSayPlugin::~AffeSayPlugin() {}

void AffeSayPlugin::info(const char *pszMsg) {
    ConMsg("%s: [INFO]  %s\n", GetPluginDescription(), pszMsg);
}

void AffeSayPlugin::debug(const char *pszMsg) {
    ConMsg("%s: [DEBUG] %s\n", GetPluginDescription(), pszMsg);
}

void AffeSayPlugin::warning(const char *pszMsg) {
    ConMsg("%s: [WARN]  %s\n", GetPluginDescription(), pszMsg);
}

void AffeSayPlugin::error(const char *pszMsg) {
    ConMsg("%s: [ERROR] %s\n", GetPluginDescription(), pszMsg);
}

// Initialize the plugin to run
// Return false if there is an error during startup.
bool AffeSayPlugin::Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory) {
    //g_pCVar = (ICvar *) interfaceFactory(CVAR_INTERFACE_VERSION, NULL);
    ConnectTier1Libraries(&interfaceFactory, 1);
    if(!g_pCVar) {
        ConMsg("%s: [ERROR] No ICvar\n", GetPluginDescription());
        return false;
    }
    g_pEngine = (IVEngineServer *) interfaceFactory(INTERFACEVERSION_VENGINESERVER, NULL);
    if(!g_pEngine)
        ConMsg("No engine\n");
    g_pPlayerInfoManager = (IPlayerInfoManager *) gameServerFactory(INTERFACEVERSION_PLAYERINFOMANAGER, NULL);
    if(!g_pPlayerInfoManager)
        ConMsg("No playerinfomanager\n");
    g_pGlobals = g_pPlayerInfoManager->GetGlobalVars();
    RegisterConCommands();
    return true;
}

// Called when the plugin should be shutdown
void AffeSayPlugin::Unload() {
}

// called when a plugins execution is stopped but the plugin is not unloaded
void AffeSayPlugin::Pause() {
}

// called when a plugin should start executing again (sometime after a Pause() call)
void AffeSayPlugin::UnPause() {
}

// Returns string describing current plugin.  e.g., Admin-Mod.  
const char *AffeSayPlugin::GetPluginDescription() {
    return "MrAffe";
}

// Called any time a new level is started (after GameInit() also on level transitions within a game)
void AffeSayPlugin::LevelInit(char const *pMapName) {
}

// The server is about to activate
void AffeSayPlugin::ServerActivate(edict_t *pEdictList, int edictCount, int clientMax) {
    g_iMaxPlayers = clientMax;
}

// The server should run physics/think on all edicts
void AffeSayPlugin::GameFrame(bool simulating) {
}

// Called when a level is shutdown (including changing levels)
void AffeSayPlugin::LevelShutdown() {
}

// Client is going active
void AffeSayPlugin::ClientActive(edict_t *pEntity) {
}

// Client is fully connected (has received initial baseline of entities)
void AffeSayPlugin::ClientFullyConnect(edict_t *pEntity) {
}

// Client is disconnecting from server
void AffeSayPlugin::ClientDisconnect(edict_t *pEntity) {
}

// Client is connected and should be put in the game
void AffeSayPlugin::ClientPutInServer(edict_t *pEntity, char const *playername) {
}

// Sets the client index for the client who typed the command into their console
void AffeSayPlugin::SetCommandClient(int index) {
}

// A player changed one/several replicated cvars (name etc)
void AffeSayPlugin::ClientSettingsChanged(edict_t *pEdict) {
}

// Client is connecting to server (set retVal to false to reject the connection)
//You can specify a rejection message by writing it into reject
PLUGIN_RESULT AffeSayPlugin::ClientConnect(bool *bAllowConnect, edict_t *pEntity, const char *pszName, const char *pszAddress, char *reject, int maxrejectlen) {
    return PLUGIN_CONTINUE;
}

// The client has typed a command at the console
PLUGIN_RESULT AffeSayPlugin::ClientCommand(edict_t *pEdict, const CCommand &args) {
    /*
    int entindex = pEdict - g_pGlobals->pEdicts;
    CCSUsrMsg_SayText msg;
    ChatFilter filter(entindex);

    ConMsg("COMMAND: %s///\n", args.GetCommandString());
    if(!strcasecmp("affe ", args.GetCommandString())) {
        ConMsg("SENDING STUFF\n");
        msg.set_text("t\x01je\x02n\x03na");
        msg.set_chat(true);
        g_pEngine->SendUserMessage(filter, CS_UM_SayText, msg);
    }
    g_pEngine->ClientPrintf(pEdict, "T\x01j\x02e\x03n\x03n\x04a\a");
    */
    return PLUGIN_CONTINUE;
}

// A user has had their network id setup and validated 
PLUGIN_RESULT AffeSayPlugin::NetworkIDValidated(const char *pszUserName, const char *pszNetworkID) {
    return PLUGIN_CONTINUE;
}

// This is called when a query from IServerPluginHelpers::StartQueryCvarValue is finished.
// iCookie is the value returned by IServerPluginHelpers::StartQueryCvarValue.
// Added with version 2 of the interface.
void AffeSayPlugin::OnQueryCvarValueFinished(QueryCvarCookie_t iCookie, edict_t *pPlayerEntity, EQueryCvarValueStatus eStatus, const char *pCvarName, const char *pCvarValue) {
}

// added with version 3 of the interface.
void AffeSayPlugin::OnEdictAllocated(edict_t *edict) {
}

void AffeSayPlugin::OnEdictFreed(const edict_t *edict) {
}	
