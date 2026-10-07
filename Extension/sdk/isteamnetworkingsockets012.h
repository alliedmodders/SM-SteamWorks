/*
 * Minimal SteamNetworkingSockets012 interface declaration.
 *
 * Steamworks SDK 1.32 does not declare ISteamNetworkingSockets. This mirrors the
 * SteamNetworkingSockets012 vtable (Steamworks SDK 1.53) up to GetFakeIP so the
 * extension can request that interface without requiring the newer SDK headers.
 * Slots before GetFakeIP are placeholders and must never be called.
 */

#pragma once

#include <steam_api.h>

#define STEAMNETWORKINGSOCKETS012_INTERFACE_VERSION "SteamNetworkingSockets012"

/* Layout-compatible with SteamNetworkingFakeIPResult_t. SteamNetworkingIdentity is
 * declared under pack(1), so the layout is the same on every platform (160 bytes). */
#pragma pack( push, 1 )
struct SWFakeIPResult_t
{
	enum { k_nMaxReturnPorts = 8 };

	EResult m_eResult;
	struct
	{
		int m_eType;
		int m_cbSize;
		uint8 m_data[128];
	} m_identity;
	uint32 m_unIP;
	uint16 m_unPorts[k_nMaxReturnPorts];
};

#pragma pack( pop )

static_assert(sizeof(SWFakeIPResult_t) == 160, "SteamNetworkingFakeIPResult_t layout mismatch");

class ISteamNetworkingSockets012
{
public:
	virtual void Unused00() = 0;
	virtual void Unused01() = 0;
	virtual void Unused02() = 0;
	virtual void Unused03() = 0;
	virtual void Unused04() = 0;
	virtual void Unused05() = 0;
	virtual void Unused06() = 0;
	virtual void Unused07() = 0;
	virtual void Unused08() = 0;
	virtual void Unused09() = 0;
	virtual void Unused10() = 0;
	virtual void Unused11() = 0;
	virtual void Unused12() = 0;
	virtual void Unused13() = 0;
	virtual void Unused14() = 0;
	virtual void Unused15() = 0;
	virtual void Unused16() = 0;
	virtual void Unused17() = 0;
	virtual void Unused18() = 0;
	virtual void Unused19() = 0;
	virtual void Unused20() = 0;
	virtual void Unused21() = 0;
	virtual void Unused22() = 0;
	virtual void Unused23() = 0;
	virtual void Unused24() = 0;
	virtual void Unused25() = 0;
	virtual void Unused26() = 0;
	virtual void Unused27() = 0;
	virtual void Unused28() = 0;
	virtual void Unused29() = 0;
	virtual void Unused30() = 0;
	virtual void Unused31() = 0;
	virtual void Unused32() = 0;
	virtual void Unused33() = 0;
	virtual void Unused34() = 0;
	virtual void Unused35() = 0;
	virtual void Unused36() = 0;
	virtual void Unused37() = 0;
	virtual void Unused38() = 0;
	virtual void Unused39() = 0;
	virtual void Unused40() = 0;
	virtual void Unused41() = 0;
	virtual void Unused42() = 0;
	virtual void GetFakeIP( int idxFirstPort, SWFakeIPResult_t *pInfo ) = 0;
};
