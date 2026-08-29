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
#include "steamworks_helpers.h"

static cell_t sm_RequestStatsAuthID(IPluginContext* pContext, const cell_t* params)
{
	ISteamGameServerStats* pStats = GetGameServerStats(pContext);
	if (pStats == NULL)
	{
		return 0;
	}

	CSteamID steamId = GetAccountId(pContext, params[1]);
	if (!steamId.IsValid())
	{
		return 0;
	}

	return pStats->RequestUserStats(steamId) != k_uAPICallInvalid ? 1 : 0;
}

static cell_t sm_RequestUserStats(IPluginContext *pContext, const cell_t *params)
{
	ISteamGameServerStats* pStats = GetGameServerStats(pContext);
	if (pStats == NULL)
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

	return pStats->RequestUserStats(steamId) != k_uAPICallInvalid ? 1 : 0;
}

static cell_t sm_GetStatCell(IPluginContext *pContext, const cell_t *params)
{
	ISteamGameServerStats* pStats = GetGameServerStats(pContext);
	if (pStats == NULL)
	{
		return 0;
	}

	IGamePlayer* pPlayer = GetValidGamePlayer(pContext, params[1]);
	if (pPlayer == NULL)
	{
		return 0;
	}
	
	char* pName;
	if (!GetStringParam(pContext, params[2], pName, INVALID_GAME_STAT))
	{
		return 0;
	}

	cell_t* pValue;
	if (!GetCellPointer(pContext, params[3], pValue))
	{
		return 0;
	}

	CSteamID steamId = GetSteamIdFromPlayer(pContext, pPlayer);
	if (!steamId.IsValid())
	{
		return 0;
	}

	return pStats->GetUserStat(steamId, pName, pValue) ? 1 : 0;
}

static cell_t sm_GetStatAuthIDCell(IPluginContext *pContext, const cell_t *params)
{
	ISteamGameServerStats* pStats = GetGameServerStats(pContext);
	if (pStats == NULL)
	{
		return 0;
	}

	char* pName;
	if (!GetStringParam(pContext, params[2], pName, INVALID_GAME_STAT))
	{
		return 0;
	}

	cell_t* pValue;
	if (!GetCellPointer(pContext, params[3], pValue))
	{
		return 0;
	}

	CSteamID steamId = GetAccountId(pContext, params[1]);
	if (!steamId.IsValid())
	{
		return 0;
	}

	return pStats->GetUserStat(steamId, pName, pValue) ? 1 : 0;
}

static cell_t sm_GetStatFloat(IPluginContext *pContext, const cell_t *params)
{
	ISteamGameServerStats* pStats = GetGameServerStats(pContext);
	if (pStats == NULL)
	{
		return 0;
	}

	IGamePlayer* pPlayer = GetValidGamePlayer(pContext, params[1]);
	if (pPlayer == NULL)
	{
		return 0;
	}

	char* pName;
	if (!GetStringParam(pContext, params[2], pName, INVALID_GAME_STAT))
	{
		return 0;
	}

	cell_t* pValue;
	if (!GetCellPointer(pContext, params[3], pValue))
	{
		return 0;
	}

	CSteamID steamId = GetSteamIdFromPlayer(pContext, pPlayer);
	if (!steamId.IsValid())
	{
		return 0;
	}

	float fValue = 0.0f;
	bool bResult = pStats->GetUserStat(steamId, pName, &fValue);
	*pValue = sp_ftoc(fValue);
	
	return bResult ? 1 : 0;
}

static cell_t sm_GetStatAuthIDFloat(IPluginContext *pContext, const cell_t *params)
{
	ISteamGameServerStats* pStats = GetGameServerStats(pContext);
	if (pStats == NULL)
	{
		return 0;
	}

	char* pName;
	if (!GetStringParam(pContext, params[2], pName, INVALID_GAME_STAT))
	{
		return 0;
	}

	cell_t* pValue;
	if (!GetCellPointer(pContext, params[3], pValue))
	{
		return 0;
	}

	CSteamID steamId = GetAccountId(pContext, params[1]);
	if (!steamId.IsValid())
	{
		return 0;
	}

	float fValue = 0.0f;
	bool bResult = pStats->GetUserStat(steamId, pName, &fValue);
	*pValue = sp_ftoc(fValue);

	return bResult ? 1 : 0;
}

static cell_t sm_IsStatsAvailable(IPluginContext* pContext, const cell_t* params)
{
	return (g_SteamWorks.pSWGameServer->GetServerStats() != NULL) ? 1 : 0;
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
