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

#include "gsnatives.h"
#include "extension.h"
#include "swgameserver.h"
#include "steamworks_helpers.h"

static cell_t sm_IsVACEnabled(IPluginContext *pContext, const cell_t *params)
{
	ISteamGameServer* pServer = GetGameServer(pContext);
	if (pServer == NULL)
	{
		return 0;
	}
	
	return pServer->BSecure() ? 1 : 0;
}

static cell_t sm_GetPublicIP(IPluginContext *pContext, const cell_t *params)
{
	ISteamGameServer* pServer = GetGameServer(pContext);
	if (pServer == NULL)
	{
		return 0;
	}

	SteamIPAddress_t sAddr = pServer->GetPublicIP();
	if (!sAddr.IsSet())
	{
		return 0;
	}

	uint32_t ipaddr = sAddr.m_unIPv4;
	
	cell_t* addr;
	if (!GetCellPointer(pContext, params[1], addr))
	{
		return 0;
	}

	for (char iter = 3; iter > -1; --iter)
	{
		addr[(~iter) & 0x03] = (static_cast<unsigned char>(ipaddr >> (iter * 8)) & 0xFF); /* I hate you; SteamTools. */
	}
	
	return 1;
}

static cell_t sm_GetPublicIPCell(IPluginContext *pContext, const cell_t *params)
{
	ISteamGameServer* pServer = GetGameServer(pContext);
	if (pServer == NULL)
	{
		return 0;
	}

	SteamIPAddress_t sAddr = pServer->GetPublicIP();
	if (!sAddr.IsSet())
	{
		return 0;
	}

	return sAddr.m_unIPv4;
}

static cell_t sm_IsLoaded(IPluginContext *pContext, const cell_t *params)
{
	return (g_SteamWorks.pSWGameServer->GetSteamClient() != NULL) ? 1 : 0;
}

static cell_t sm_SetGameData(IPluginContext *pContext, const cell_t *params)
{
	ISteamGameServer* pServer = GetGameServer(pContext);
	if (pServer == NULL)
	{
		return 0;
	}
	
	char* pData;
	if (!GetStringParam(pContext, params[1], pData, "Invalid or empty game data"))
	{
		return 0;
	}

	pServer->SetGameData(pData);
	return 1;
}

static cell_t sm_SetGameDescription(IPluginContext *pContext, const cell_t *params)
{
	ISteamGameServer* pServer = GetGameServer(pContext);
	if (pServer == NULL)
	{
		return 0;
	}
	
	char* pDesc;
	if (!GetStringParam(pContext, params[1], pDesc, "Invalid or empty game description"))
	{
		return 0;
	}

	pServer->SetGameDescription(pDesc);
	return 1;
}

static cell_t sm_SetMapName(IPluginContext *pContext, const cell_t *params)
{
	ISteamGameServer* pServer = GetGameServer(pContext);
	if (pServer == NULL)
	{
		return 0;
	}
	
	char* pMapName;
	if (!GetStringParam(pContext, params[1], pMapName, "Invalid or empty map name"))
	{
		return 0;
	}

	pServer->SetMapName(pMapName);
	return 1;
}

static cell_t sm_IsConnected(IPluginContext *pContext, const cell_t *params)
{
	ISteamGameServer* pServer = GetGameServer(pContext);
	if (pServer == NULL)
	{
		return 0;
	}

	return pServer->BLoggedOn() ? 1 : 0;
}

static cell_t sm_SetRule(IPluginContext *pContext, const cell_t *params)
{
	ISteamGameServer* pServer = GetGameServer(pContext);
	if (pServer == NULL)
	{
		return 0;
	}

	char* pKey, *pValue;
	if (!GetStringParam(pContext, params[1], pKey, "Invalid or empty rule key"))
	{
		return 0;
	}

	if (!GetStringParam(pContext, params[2], pValue, "Invalid or empty rule value"))
	{
		return 0;
	}

	pServer->SetKeyValue(pKey, pValue);
	return 1;
}

static cell_t sm_ClearRules(IPluginContext *pContext, const cell_t *params)
{
	ISteamGameServer* pServer = GetGameServer(pContext);
	if (pServer == NULL)
	{
		return 0;
	}

	pServer->ClearAllKeyValues();
	return 1;
}

static cell_t sm_SetAdvertiseServerActive(IPluginContext *pContext, const cell_t *params)
{
	ISteamGameServer* pServer = GetGameServer(pContext);
	if (pServer == NULL)
	{
		return 0;
	}

	pServer->SetAdvertiseServerActive(!!params[1]);
	return 1;
}

static cell_t sm_ForceHeartbeat(IPluginContext *pContext, const cell_t *params)
{
	/* Deprecated no-op: newer Steamworks SDKs removed ISteamGameServer::ForceHeartbeat();
	   server list heartbeats are now sent implicitly by Steam. */
	return 0;
}

static cell_t sm_UserHasLicenseForApp(IPluginContext *pContext, const cell_t *params)
{
	ISteamGameServer* pServer = GetGameServer(pContext);
	if (pServer == NULL)
	{
		return 0;
	}

	IGamePlayer* pPlayer = GetValidGamePlayer(pContext, params[1]);
	if (pPlayer == NULL)
	{
		return 0;
	}

	CSteamID steamId = GetSteamIdFromPlayer(pContext, pPlayer);
	if (!steamId.IsValid())
	{
		return 0;
	}

	return pServer->UserHasLicenseForApp(steamId, params[2]);
}

static cell_t sm_UserHasLicenseForAppId(IPluginContext *pContext, const cell_t *params)
{
	ISteamGameServer* pServer = GetGameServer(pContext);
	if (pServer == NULL)
	{
		return 0;
	}

	CSteamID steamId = GetAccountId(pContext, params[1]);
	if (!steamId.IsValid())
	{
		return 0;
	}

	return pServer->UserHasLicenseForApp(steamId, params[2]);
}

static cell_t sm_GetClientSteamID(IPluginContext *pContext, const cell_t *params)
{
	IGamePlayer* pPlayer = GetValidGamePlayer(pContext, params[1]);
	if (pPlayer == NULL)
	{
		return 0;
	}

	CSteamID steamId = GetSteamIdFromPlayer(pContext, pPlayer);
	if (!steamId.IsValid())
	{
		return 0;
	}

	int bufferSize = params[3];
	char* steamIdBuffer;
	if (!GetBufferPointer(pContext, params[2], steamIdBuffer, bufferSize))
	{
		return 0;
	}

	int numBytes = g_pSM->Format(steamIdBuffer, params[3], "%llu", steamId.ConvertToUint64());
	numBytes++; // account for null terminator
	
	return numBytes;
}

static cell_t sm_GetUserGroupStatus(IPluginContext *pContext, const cell_t *params)
{
	ISteamGameServer* pServer = GetGameServer(pContext);
	if (pServer == NULL)
	{
		return 0;
	}

	IGamePlayer* pPlayer = GetValidGamePlayer(pContext, params[1]);
	if (pPlayer == NULL)
	{
		return 0;
	}

	CSteamID steamId = GetSteamIdFromPlayer(pContext, pPlayer);
	if (!steamId.IsValid())
	{
		return 0;
	}

	CSteamID groupSteamId = GetAccountId(pContext, params[2], k_EAccountTypeClan);
	if (!groupSteamId.IsValid())
	{
		return 0;
	}

	return pServer->RequestUserGroupStatus(steamId, groupSteamId);
}

static cell_t sm_GetUserGroupStatusAuthID(IPluginContext *pContext, const cell_t *params)
{
	ISteamGameServer* pServer = GetGameServer(pContext);
	if (pServer == NULL)
	{
		return 0;
	}

	CSteamID steamId = GetAccountId(pContext, params[1]);
	if (!steamId.IsValid())
	{
		return 0;
	}

	CSteamID groupSteamId = GetAccountId(pContext, params[2], k_EAccountTypeClan);
	if (!groupSteamId.IsValid())
	{
		return 0;
	}

	return pServer->RequestUserGroupStatus(steamId, groupSteamId);
}

static cell_t sm_IsGameServerAvailable(IPluginContext* pContext, const cell_t* params)
{
	return (g_SteamWorks.pSWGameServer->GetGameServer() != NULL) ? 1 : 0;
}

static sp_nativeinfo_t gsnatives[] =
{
	{"SteamWorks_IsGameServerAvailable",			sm_IsGameServerAvailable},
	{"SteamWorks_IsVACEnabled",						sm_IsVACEnabled},
	{"SteamWorks_GetPublicIP",						sm_GetPublicIP},
	{"SteamWorks_GetPublicIPCell",					sm_GetPublicIPCell},
	{"SteamWorks_IsLoaded",							sm_IsLoaded},
	{"SteamWorks_SetGameData",						sm_SetGameData},
	{"SteamWorks_SetGameDescription",				sm_SetGameDescription},
	{"SteamWorks_SetMapName",						sm_SetMapName},
	{"SteamWorks_IsConnected",						sm_IsConnected},
	{"SteamWorks_SetRule",							sm_SetRule},
	{"SteamWorks_ClearRules",						sm_ClearRules},
	{"SteamWorks_SetAdvertiseServerActive",			sm_SetAdvertiseServerActive},
	{"SteamWorks_ForceHeartbeat",					sm_ForceHeartbeat},
	{"SteamWorks_HasLicenseForApp",					sm_UserHasLicenseForApp},
	{"SteamWorks_HasLicenseForAppId",				sm_UserHasLicenseForAppId},
	{"SteamWorks_GetClientSteamID",					sm_GetClientSteamID},
	{"SteamWorks_GetUserGroupStatus",				sm_GetUserGroupStatus},
	{"SteamWorks_GetUserGroupStatusAuthID",			sm_GetUserGroupStatusAuthID},
	{NULL,											NULL}
};

SteamWorksGSNatives::SteamWorksGSNatives()
{
	sharesys->AddNatives(myself, gsnatives);
}

SteamWorksGSNatives::~SteamWorksGSNatives()
{
	/* We tragically can't remove ourselves... hopefully no one uses this class, you know, like a class. */
}
