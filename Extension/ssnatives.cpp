/*
    This file is part of SourcePawn SteamWorks.

    SourcePawn SteamWorks is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, as per version 3 of the License.

    SourcePawn SteamWorks is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SourcePawn SteamWorks.  If not, see <http://www.gnu.org/licenses/>.
	
	Author: Kyle Sanderson (KyleS).
*/

#include "ssnatives.h"
#include "extension.h"
#include <steam_gameserver.h>
#include "swgameserver.h"

static bool IsSteamWorksLoaded(void)
{
	return (g_SteamWorks.pSWGameServer->GetSteamClient() != NULL);
}

static ISteamGameServerStats *GetServerStatsPointer(void)
{
	return g_SteamWorks.pSWGameServer->GetServerStats();
}

static CSteamID CreateCommonCSteamID(IGamePlayer *pPlayer, const cell_t *params, unsigned char universeplace = 2, unsigned char typeplace = 3)
{
	return g_SteamWorks.CreateCommonCSteamID(pPlayer, params, universeplace, typeplace);
}

static CSteamID CreateCommonCSteamID(uint32_t authid, const cell_t *params, unsigned char universeplace = 2, unsigned char typeplace = 3)
{
	return g_SteamWorks.CreateCommonCSteamID(authid, params, universeplace, typeplace);
}

static cell_t sm_RequestStatsAuthID(IPluginContext *pContext, const cell_t *params)
{
	ISteamGameServerStats *pStats = GetServerStatsPointer();
	if (pStats == NULL)
	{
		return pContext->ThrowNativeError("ISteamGameServerStats interface not available");
	}

	uint32_t authid = params[1];
	CSteamID checkid = CreateCommonCSteamID(authid, params);
	if (!checkid.IsValid())
	{
		return pContext->ThrowNativeError("Failed to get valid SteamID for authid %u", authid);
	}

	return pStats->RequestUserStats(checkid) != k_uAPICallInvalid ? 1 : 0;
}

static cell_t sm_RequestUserStats(IPluginContext *pContext, const cell_t *params)
{
	ISteamGameServerStats *pStats = GetServerStatsPointer();
	if (pStats == NULL)
	{
		return pContext->ThrowNativeError("ISteamGameServerStats interface not available");
	}

	int client = params[1];
	if (client < 1 || client > playerhelpers->GetMaxClients())
	{
		return pContext->ThrowNativeError("Client index %d is invalid", client);
	}

	IGamePlayer *pPlayer = playerhelpers->GetGamePlayer(client);
	if (pPlayer == NULL || !pPlayer->IsConnected())
	{
		return pContext->ThrowNativeError("Client index %d is not connected", client);
	}

	CSteamID checkid = CreateCommonCSteamID(pPlayer, params);
	if (!checkid.IsValid())
	{
		return pContext->ThrowNativeError("Failed to get valid SteamID for client %d", client);
	}

	return pStats->RequestUserStats(checkid) != k_uAPICallInvalid ? 1 : 0;
}

static cell_t sm_GetStatCell(IPluginContext *pContext, const cell_t *params)
{
	ISteamGameServerStats *pStats = GetServerStatsPointer();
	if (pStats == NULL)
	{
		return pContext->ThrowNativeError("ISteamGameServerStats interface not available");
	}

	int client = params[1];
	if (client < 1 || client > playerhelpers->GetMaxClients())
	{
		return pContext->ThrowNativeError("Client index %d is invalid", client);
	}

	IGamePlayer *pPlayer = playerhelpers->GetGamePlayer(client);
	if (pPlayer == NULL || !pPlayer->IsConnected())
	{
		return pContext->ThrowNativeError("Client index %d is not connected", client);
	}
	
	char *pName;
	if (pContext->LocalToString(params[2], &pName) != SP_ERROR_NONE || pName == NULL || pName[0] == '\0')
	{
		return pContext->ThrowNativeError("Invalid or empty stat name");
	}

	cell_t* pValue;
	if (pContext->LocalToPhysAddr(params[3], &pValue) != SP_ERROR_NONE || pValue == NULL)
	{
		return pContext->ThrowNativeError("Invalid pointer for result value");
	}

	CSteamID checkid = CreateCommonCSteamID(pPlayer, params, 4, 5);
	if (!checkid.IsValid())
	{
		return pContext->ThrowNativeError("Failed to get valid SteamID for client %d", client);
	}

	return pStats->GetUserStat(checkid, pName, pValue) ? 1 : 0;
}

static cell_t sm_GetStatAuthIDCell(IPluginContext *pContext, const cell_t *params)
{
	ISteamGameServerStats *pStats = GetServerStatsPointer();
	if (pStats == NULL)
	{
		return pContext->ThrowNativeError("ISteamGameServerStats interface not available");
	}

	char* pName;
	if (pContext->LocalToString(params[2], &pName) != SP_ERROR_NONE || pName == NULL || pName[0] == '\0')
	{
		return pContext->ThrowNativeError("Invalid or empty stat name");
	}

	cell_t* pValue;
	if (pContext->LocalToPhysAddr(params[3], &pValue) != SP_ERROR_NONE || pValue == NULL)
	{
		return pContext->ThrowNativeError("Invalid pointer for result value");
	}

	uint32_t authid = params[1];
	CSteamID checkid = CreateCommonCSteamID(authid, params, 4, 5);
	if (!checkid.IsValid())
	{
		return pContext->ThrowNativeError("Failed to get valid SteamID for authid %u", authid);
	}

	return pStats->GetUserStat(checkid, pName, pValue) ? 1 : 0;
}

static cell_t sm_GetStatFloat(IPluginContext *pContext, const cell_t *params)
{
	ISteamGameServerStats *pStats = GetServerStatsPointer();
	if (pStats == NULL)
	{
		return pContext->ThrowNativeError("ISteamGameServerStats interface not available");
	}

	int client = params[1];
	if (client < 1 || client > playerhelpers->GetMaxClients())
	{
		return pContext->ThrowNativeError("Client index %d is invalid", client);
	}

	IGamePlayer *pPlayer = playerhelpers->GetGamePlayer(client);
	if (pPlayer == NULL || !pPlayer->IsConnected())
	{
		return pContext->ThrowNativeError("Client index %d is not connected", client);
	}
	
	char* pName;
	if (pContext->LocalToString(params[2], &pName) != SP_ERROR_NONE || pName == NULL || pName[0] == '\0')
	{
		return pContext->ThrowNativeError("Invalid or empty stat name");
	}

	cell_t* pValue;
	if (pContext->LocalToPhysAddr(params[3], &pValue) != SP_ERROR_NONE || pValue == NULL)
	{
		return pContext->ThrowNativeError("Invalid pointer for result value");
	}

	CSteamID checkid = CreateCommonCSteamID(pPlayer, params, 4, 5);
	if (!checkid.IsValid())
	{
		return pContext->ThrowNativeError("Failed to get valid SteamID for client %d", client);
	}

	float fValue;
	bool bResult = pStats->GetUserStat(checkid, pName, &fValue);
	*pValue = sp_ftoc(fValue);
	
	return bResult ? 1 : 0;
}

static cell_t sm_GetStatAuthIDFloat(IPluginContext *pContext, const cell_t *params)
{
	ISteamGameServerStats *pStats = GetServerStatsPointer();
	if (pStats == NULL)
	{
		return pContext->ThrowNativeError("ISteamGameServerStats interface not available");
	}

	char* pName;
	if (pContext->LocalToString(params[2], &pName) != SP_ERROR_NONE || pName == NULL || pName[0] == '\0')
	{
		return pContext->ThrowNativeError("Invalid or empty stat name");
	}

	cell_t* pValue;
	if (pContext->LocalToPhysAddr(params[3], &pValue) != SP_ERROR_NONE || pValue == NULL)
	{
		return pContext->ThrowNativeError("Invalid pointer for result value");
	}

	uint32_t authid = params[1];
	CSteamID checkid = CreateCommonCSteamID(authid, params, 4, 5);
	if (!checkid.IsValid())
	{
		return pContext->ThrowNativeError("Failed to get valid SteamID for authid %u", authid);
	}

	float fValue;
	bool bResult = pStats->GetUserStat(checkid, pName, &fValue);
	*pValue = sp_ftoc(fValue);

	return bResult ? 1 : 0;
}

static cell_t sm_IsStatsAvailable(IPluginContext* pContext, const cell_t* params)
{
	return (GetServerStatsPointer() != NULL) ? 1 : 0;
}

static sp_nativeinfo_t ssnatives[] =
{
	{"SteamWorks_IsStatsAvailable",					sm_IsStatsAvailable},
	{"SteamWorks_RequestStatsAuthID",				sm_RequestStatsAuthID},
	{"SteamWorks_RequestStats",						sm_RequestUserStats},
	{"SteamWorks_GetStatCell",						sm_GetStatCell},
	{"SteamWorks_GetStatAuthIDCell",				sm_GetStatAuthIDCell},
	{"SteamWorks_GetStatFloat",						sm_GetStatFloat},
	{"SteamWorks_GetStatAuthIDFloat",				sm_GetStatAuthIDFloat},
	{NULL,											NULL}
};

SteamWorksSSNatives::SteamWorksSSNatives()
{
	sharesys->AddNatives(myself, ssnatives);
}

SteamWorksSSNatives::~SteamWorksSSNatives()
{
	/* We tragically can't remove ourselves... hopefully no one uses this class, you know, like a class. */
}
