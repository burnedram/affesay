//#include <convar.h>
#include "affesayplugin.h"
#include "recipientfilters.h"
#include "cstrike15_usermessages.pb.h"

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

void RegisterConCommands() {
    g_pCVar->RegisterConCommand(&myVar);
    g_pCVar->RegisterConCommand(&myComm);
    g_pCVar->RegisterConCommand(&affesay_name);
}
