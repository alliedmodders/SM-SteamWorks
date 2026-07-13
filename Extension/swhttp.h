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

#include <steam_gameserver.h>
#include <isteamhttp.h>
#include <smsdk_ext.h>

#include <unordered_map>

class SteamWorksHTTPRequest;

class SteamWorksHTTP :
	public IHandleTypeDispatch
{
	public:
		SteamWorksHTTP();
		~SteamWorksHTTP();

	public:
		void OnHandleDestroy(HandleType_t type, void *object);
		bool GetHandleApproxSize(HandleType_t type, void *object, unsigned int *pSize);

	public:
		HandleType_t GetHTTPHandle(void);

		/* Streaming responses report progress through HTTPRequestHeadersReceived_t /
		   HTTPRequestDataReceived_t. Steam delivers those as broadcast gameserver
		   callbacks (not as call results of the streaming API call), so a single
		   dispatcher here receives them and routes each one to the owning request by
		   its handle. Requests add/remove themselves as they are created/destroyed. */
		void RegisterRequest(SteamWorksHTTPRequest *pRequest);
		void UnregisterRequest(SteamWorksHTTPRequest *pRequest);

	private:
		SteamWorksHTTPRequest *FindRequest(HTTPRequestHandle request);

		STEAM_GAMESERVER_CALLBACK(SteamWorksHTTP, OnHTTPHeadersReceived, HTTPRequestHeadersReceived_t, m_CallbackHeadersReceived);
		STEAM_GAMESERVER_CALLBACK(SteamWorksHTTP, OnHTTPDataReceived, HTTPRequestDataReceived_t, m_CallbackDataReceived);

	private:
		HandleType_t typeHTTP;
		std::unordered_map<HTTPRequestHandle, SteamWorksHTTPRequest *> m_Requests;
};
