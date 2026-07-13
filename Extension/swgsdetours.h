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

#pragma once

#include <CDetour/detours.h>

class CDetour;

/* SourceMod's safetyhook-based CDetour doesn't ship the "fixed address" helper the
   old vendored copy had, so define it in terms of the address overload it does expose. */
#ifndef DETOUR_CREATE_STATIC_FIXED
#define DETOUR_CREATE_STATIC_FIXED(name, address) CDetourManager::CreateDetour(GET_STATIC_CALLBACK(name), GET_STATIC_TRAMPOLINE(name), address);
#endif

class SteamWorksGSDetours
{
	public:
		SteamWorksGSDetours();
		~SteamWorksGSDetours();
		
	private:
		CDetour *m_pSafeInitDetour;
		CDetour *m_pShutdownDetour;
};
