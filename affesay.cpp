//#include <convar.h>
#include "affesayplugin.h"
#include "recipientfilters.h"
#include "cstrike15_usermessages.pb.h"
#include "cplayerinfo.h"

ConVar myVar("myVar", "42", FCVAR_REPLICATED | FCVAR_NOTIFY, "A number...");
void MyFun() {
    DEBUG("HELLO FROM THE OUTSIIIIIIIDEEEEEEEE");
    for(int i = 1; i <= g_iMaxPlayers; i++) {
        IPlayerInfo *info = g_pPlayerInfoManager->GetPlayerInfo(g_pGlobals->pEdicts + i);
        if(info)
            DEBUG("\tPlayer %d %s %s\n", i, info->GetName(), info->GetNetworkIDString());
    }
}
ConCommand myComm("myComm", MyFun, "Theo pizza pls", 0);

// Sends pszMessage to the first user that has the name pszName
void SayName(const char *pszName, const char *pszMessage) {
    RecipientFilter filter(1);
    for(int i = 1; i <= g_iMaxPlayers; i++) {
        IPlayerInfo *info = g_pPlayerInfoManager->GetPlayerInfo(g_pGlobals->pEdicts + i);
        if(info && !strcmp(info->GetName(), pszName)) {
            filter.AddRecipient(i);
            break;
        }
    }
    filter.MakeReliable();
    if(!filter.GetRecipientCount()) {
        ConMsg("no user with that name\n");
        return;
    }

    CCSUsrMsg_SayText msg;
    msg.set_text(pszMessage);
    msg.set_chat(true);
    g_pEngine->SendUserMessage(filter, CS_UM_SayText, msg);

    /*
    // Send the same message but in rainbow colors. Sends weird chars for long strings
    char pszColored[strlen(pszMessage) * 2 + 1];
    for(unsigned int i = 0, j = 0; i < strlen(pszMessage);) {
        pszColored[j++] = i+1;
        pszColored[j++] = pszMessage[i++];
    }
    pszColored[strlen(pszMessage) * 2] = 0;

    CCSUsrMsg_SayText msg2;
    msg2.set_text(pszColored);
    msg.set_chat(true);
    g_pEngine->SendUserMessage(filter, CS_UM_SayText, msg2);
    */
}
void cc_SayName(const CCommand &args) {
    if(args.ArgC() < 3) {
        ConMsg("Usage: affesay_name NAME MESSAGE\n");
        return;
    }
    const char *pszName = args.Arg(1);
    const char *pszMessage = args.ArgS();
    // This doesn't feel right, but since we should never worry about
    // deleting the string this shouldn't be a problem
    pszMessage += strlen(pszName) + 1; // + 1 for whitespace
    SayName(pszName, pszMessage);
}
ConCommand affesay_name("affesay_name", cc_SayName, "Send a chat message to a player with a particular name", FCVAR_SERVER_CAN_EXECUTE);

void PlayersJSON() {
    ConMsg("{\"players\":[\n");
    for(int i = 1; i <= g_iMaxPlayers; i++) {
        CPlayerInfo *info = (CPlayerInfo *)g_pPlayerInfoManager->GetPlayerInfo(g_pGlobals->pEdicts + i);
        if(!info || info->IsFakeClient())
            continue;
        ConMsg("  {\n");
        ConMsg("    \"name\":"              "\"%s\",\n", info->GetName());
        ConMsg("    \"entindex\":"          "\"%d\",\n", i);
        ConMsg("    \"slot\":"              "\"%d\",\n", info->GetUserID());
        ConMsg("    \"networkID\":"         "\"%s\",\n", info->GetNetworkIDString());
        ConMsg("    \"teamIndex\":"         "\"%d\",\n", info->GetTeamIndex());
        ConMsg("    \"fragCount\":"         "\"%d\",\n", info->GetFragCount());
        ConMsg("    \"assistsCount\":"      "\"%d\",\n", info->GetAssistsCount());
        ConMsg("    \"deathCount\":"        "\"%d\",\n", info->GetDeathCount());
        ConMsg("    \"isConnected\":"       "\"%s\",\n", info->IsConnected() ? "true" : "false");
        ConMsg("    \"armor\":"             "\"%d\",\n", info->GetArmorValue());
        ConMsg("    \"isHLTV\":"            "\"%s\",\n", info->IsHLTV() ? "true" : "false");
        ConMsg("    \"isPlayer\":"          "\"%s\",\n", info->IsPlayer() ? "true" : "false");
        ConMsg("    \"isFakeClient\":"      "\"%s\",\n", info->IsFakeClient() ? "true" : "false");
        ConMsg("    \"isDead\":"            "\"%s\",\n", info->IsDead() ? "true" : "false");
        ConMsg("    \"isInAVehicle\":"      "\"%s\",\n", info->IsInAVehicle() ? "true" : "false");
        ConMsg("    \"isObserver\":"        "\"%s\",\n", info->IsObserver() ? "true" : "false");
        const Vector absOrigin = info->GetAbsOrigin();
        ConMsg("    \"absOrigin\":{\n");
        ConMsg("      \"x\":"               "\"%f\",\n", absOrigin.x);
        ConMsg("      \"y\":"               "\"%f\",\n", absOrigin.y);
        ConMsg("      \"z\":"               "\"%f\"\n",  absOrigin.z);
        ConMsg("    },\n");
        const QAngle absAngles = info->GetAbsAngles();
        ConMsg("    \"absAngles\":{\n");
        ConMsg("      \"pitch\":"           "\"%f\",\n", absAngles.x);
        ConMsg("      \"yaw\":"             "\"%f\",\n", absAngles.y);
        ConMsg("      \"roll\":"            "\"%f\"\n",  absAngles.z);
        ConMsg("    },\n");
        const Vector playerMins = info->GetPlayerMins();
        ConMsg("    \"playerMins\":{\n");
        ConMsg("      \"x\":"               "\"%f\",\n", playerMins.x);
        ConMsg("      \"y\":"               "\"%f\",\n", playerMins.y);
        ConMsg("      \"z\":"               "\"%f\"\n",  playerMins.z);
        ConMsg("    },\n");
        const Vector playerMaxs = info->GetPlayerMaxs();
        ConMsg("    \"playerMaxs\":""{\n");
        ConMsg("      \"x\":"               "\"%f\",\n", playerMaxs.x);
        ConMsg("      \"y\":"               "\"%f\",\n", playerMaxs.y);
        ConMsg("      \"z\":"               "\"%f\"\n",  playerMaxs.z);
        ConMsg("    },\n");
        ConMsg("    \"weaponName\":"        "\"%s\",\n", info->GetWeaponName());
        ConMsg("    \"modelName\":"         "\"%s\",\n", info->GetModelName());
        ConMsg("    \"health\":"            "\"%d\",\n", info->GetHealth());
        ConMsg("    \"maxHealth\":"         "\"%d\",\n", info->GetMaxHealth());
        CBotCmd lastUserCommand = info->GetLastUserCommand();
        ConMsg("    \"lastUserCommand\":{\n");
        ConMsg("      \"commandNumber\":"   "\"%d\",\n", lastUserCommand.command_number);
        ConMsg("      \"tickCount\":"       "\"%d\",\n", lastUserCommand.tick_count);
        QAngle viewAngles = lastUserCommand.viewangles;
        ConMsg("      \"absAngles\":{\n");
        ConMsg("        \"pitch\":"         "\"%f\",\n", viewAngles.x);
        ConMsg("        \"yaw\":"           "\"%f\",\n", viewAngles.y);
        ConMsg("        \"roll\":"          "\"%f\"\n",  viewAngles.z);
        ConMsg("      },\n");
        ConMsg("      \"forwardMove\":"     "\"%f\",\n", lastUserCommand.forwardmove);
        ConMsg("      \"sideMove\":"        "\"%f\",\n", lastUserCommand.sidemove);
        ConMsg("      \"upMove\":"          "\"%f\",\n", lastUserCommand.upmove);
        ConMsg("      \"buttons\":"         "\"%d\",\n", lastUserCommand.buttons);
        ConMsg("      \"impulse\":"         "\"%d\",\n", lastUserCommand.impulse);
        ConMsg("      \"weaponSelect\":"    "\"%d\",\n", lastUserCommand.weaponselect);
        ConMsg("      \"weaponSubtype\":"   "\"%d\",\n", lastUserCommand.weaponsubtype);
        ConMsg("      \"randomSeed\":"      "\"%d\",\n", lastUserCommand.random_seed);
        ConMsg("      \"mouseDX\":"         "\"%d\",\n", lastUserCommand.mousedx);
        ConMsg("      \"mouseDY\":"         "\"%d\",\n", lastUserCommand.mousedy);
        ConMsg("      \"hasBeenPredicted\":""\"%s\"\n",  lastUserCommand.hasbeenpredicted ? "true" : "false");
        ConMsg("    }\n");
        if(i == g_iMaxPlayers)
            ConMsg("  }\n");
        else
            ConMsg("  },\n");
    }
    ConMsg("]}\n");
}
void cc_PlayersJSON(const CCommand &args) {
    if(args.ArgC() > 1) {
        ConMsg("Usage: affesay_players_json");
        return;
    }
    PlayersJSON();
}
ConCommand affesay_listids("affesay_players_json", cc_PlayersJSON, "Lists all players in JSON format", FCVAR_SERVER_CAN_EXECUTE);

void RegisterConCommands() {
    g_pCVar->RegisterConCommand(&myVar);
    g_pCVar->RegisterConCommand(&myComm);
    g_pCVar->RegisterConCommand(&affesay_name);
    g_pCVar->RegisterConCommand(&affesay_listids);
}
