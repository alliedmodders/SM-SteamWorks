/*
 * Minimal SteamGameServer014 interface declaration.
 *
 * Steamworks SDK 1.32 only declares SteamGameServer012. This prefix mirrors
 * SteamGameServer014 through SetAdvertiseServerActive so the extension can
 * request that interface without requiring the newer SDK headers.
 */

#pragma once

#include <steam_api.h>

#define STEAMGAMESERVER014_INTERFACE_VERSION "SteamGameServer014"

class ISteamGameServer014
{
public:
	virtual bool InitGameServer(uint32 unIP, uint16 usGamePort, uint16 usQueryPort,
		uint32 unFlags, AppId_t nGameAppId, const char *pchVersionString) = 0;
	virtual void SetProduct(const char *pszProduct) = 0;
	virtual void SetGameDescription(const char *pszGameDescription) = 0;
	virtual void SetModDir(const char *pszModDir) = 0;
	virtual void SetDedicatedServer(bool bDedicated) = 0;
	virtual void LogOn(const char *pszToken) = 0;
	virtual void LogOnAnonymous() = 0;
	virtual void LogOff() = 0;
	virtual bool BLoggedOn() = 0;
	virtual bool BSecure() = 0;
	virtual CSteamID GetSteamID() = 0;
	virtual bool WasRestartRequested() = 0;
	virtual void SetMaxPlayerCount(int cPlayersMax) = 0;
	virtual void SetBotPlayerCount(int cBotplayers) = 0;
	virtual void SetServerName(const char *pszServerName) = 0;
	virtual void SetMapName(const char *pszMapName) = 0;
	virtual void SetPasswordProtected(bool bPasswordProtected) = 0;
	virtual void SetSpectatorPort(uint16 unSpectatorPort) = 0;
	virtual void SetSpectatorServerName(const char *pszSpectatorServerName) = 0;
	virtual void ClearAllKeyValues() = 0;
	virtual void SetKeyValue(const char *pKey, const char *pValue) = 0;
	virtual void SetGameTags(const char *pchGameTags) = 0;
	virtual void SetGameData(const char *pchGameData) = 0;
	virtual void SetRegion(const char *pszRegion) = 0;
	virtual void SetAdvertiseServerActive(bool bActive) = 0;
};
