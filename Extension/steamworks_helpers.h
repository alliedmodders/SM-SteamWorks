#pragma once

#include "extension.h"

#define INVALID_GAME_STAT		"Invalid or empty game stat name"

inline ISteamGameServerStats* GetGameServerStats(IPluginContext* pContext)
{
	ISteamGameServerStats* pStats = g_SteamWorks.pSWGameServer->GetServerStats();
	if (pStats == NULL)
	{
		pContext->ThrowNativeError("ISteamGameServerStats interface not available");
		return NULL;
	}

	return pStats;
}

inline ISteamGameServer* GetGameServer(IPluginContext* pContext)
{
	ISteamGameServer* pServer = g_SteamWorks.pSWGameServer->GetGameServer();
	if (pServer == NULL)
	{
		pContext->ThrowNativeError("ISteamGameServer interface not available");
		return NULL;
	}

	return pServer;
}

inline CSteamID GetAccountId(IPluginContext* pContext, uint32_t accountId, EAccountType accountType = k_EAccountTypeIndividual)
{
	if (accountId == 0)
	{
		pContext->ThrowNativeError("Invalid steam account id received %d", accountId);
		return CSteamID();
	}

	CSteamID steamId(accountId, k_EUniversePublic, accountType);
	if (!steamId.IsValid())
	{
		pContext->ThrowNativeError("Failed to create valid class 'CSteamID' for steam account id %d", accountId);
		return steamId;
	}

	return steamId;
}

inline IGamePlayer* GetValidGamePlayer(IPluginContext* pContext, int clientIndex)
{
	if (clientIndex < 1 || clientIndex > playerhelpers->GetMaxClients())
	{
		pContext->ThrowNativeError("Client index %d is invalid", clientIndex);
		return NULL;
	}

	IGamePlayer* pPlayer = playerhelpers->GetGamePlayer(clientIndex);
	if (pPlayer == NULL || !pPlayer->IsConnected())
	{
		pContext->ThrowNativeError("Client index %d is not connected", clientIndex);
		return NULL;
	}

	return pPlayer;
}

inline bool GetStringParam(IPluginContext* pContext, cell_t param, char*& outStr, const char* errorMsg = "Invalid or empty string parameter")
{
	if (pContext->LocalToString(param, &outStr) != SP_ERROR_NONE || outStr == NULL || outStr[0] == '\0')
	{
		pContext->ThrowNativeError("%s", errorMsg);
		return false;
	}

	return true;
}

inline bool GetBufferPointer(IPluginContext* pContext, cell_t param, char*& outBuffer, int outSize)
{
	if (pContext->LocalToString(param, &outBuffer) != SP_ERROR_NONE || outBuffer == NULL)
	{
		pContext->ThrowNativeError("Invalid buffer pointer");
		return false;
	}

	if (outSize < 1)
	{
		pContext->ThrowNativeError("Buffer size must be at least 1");
		return false;
	}

	return true;
}

inline bool GetCellPointer(IPluginContext* pContext, cell_t param, cell_t*& outPtr)
{
	if (pContext->LocalToPhysAddr(param, &outPtr) != SP_ERROR_NONE || outPtr == NULL)
	{
		pContext->ThrowNativeError("Invalid pointer for result value");
		return false;
	}

	return true;
}

inline CSteamID GetSteamIdFromPlayer(IPluginContext* pContext, IGamePlayer* pPlayer)
{
	uint32_t accountId = pPlayer->GetSteamAccountID(false);
	return GetAccountId(pContext, accountId);
}