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

#include "swhttp.h"

static ISteamHTTP *GetHTTPPointer()
{
	return g_SteamWorks.pSWGameServer->GetHTTP();
}

SteamWorksHTTP::SteamWorksHTTP() :
	m_CallbackHeadersReceived(this, &SteamWorksHTTP::OnHTTPHeadersReceived),
	m_CallbackDataReceived(this, &SteamWorksHTTP::OnHTTPDataReceived)
{
	this->typeHTTP = handlesys->CreateType("HTTPHandle", this, 0, NULL, NULL, myself->GetIdentity(), NULL);
}

SteamWorksHTTP::~SteamWorksHTTP()
{
	handlesys->RemoveType(this->typeHTTP, myself->GetIdentity());
}

HandleType_t SteamWorksHTTP::GetHTTPHandle(void)
{
	return this->typeHTTP;
}

void SteamWorksHTTP::RegisterRequest(SteamWorksHTTPRequest *pRequest)
{
	if (pRequest->request != INVALID_HTTPREQUEST_HANDLE)
	{
		this->m_Requests[pRequest->request] = pRequest;
	}
}

void SteamWorksHTTP::UnregisterRequest(SteamWorksHTTPRequest *pRequest)
{
	if (pRequest->request != INVALID_HTTPREQUEST_HANDLE)
	{
		this->m_Requests.erase(pRequest->request);
	}
}

SteamWorksHTTPRequest *SteamWorksHTTP::FindRequest(HTTPRequestHandle request)
{
	auto it = this->m_Requests.find(request);
	return (it == this->m_Requests.end()) ? NULL : it->second;
}

void SteamWorksHTTP::OnHTTPHeadersReceived(HTTPRequestHeadersReceived_t *pParam)
{
	SteamWorksHTTPRequest *pRequest = this->FindRequest(pParam->m_hRequest);
	if (pRequest != NULL)
	{
		pRequest->OnHTTPHeadersReceived(pParam);
	}
}

void SteamWorksHTTP::OnHTTPDataReceived(HTTPRequestDataReceived_t *pParam)
{
	SteamWorksHTTPRequest *pRequest = this->FindRequest(pParam->m_hRequest);
	if (pRequest != NULL)
	{
		pRequest->OnHTTPDataReceived(pParam);
	}
}

static void DelayedDeleteSteamWorksHTTPRequest(void *object)
{
	SteamWorksHTTPRequest *pRequest = reinterpret_cast<SteamWorksHTTPRequest *>(object);
	delete pRequest;
}

void SteamWorksHTTP::OnHandleDestroy(HandleType_t type, void *object)
{
	if (type == this->typeHTTP) /* Heaven forbid we ever offer another handle. */
	{
		smutils->AddFrameAction(DelayedDeleteSteamWorksHTTPRequest, object);
	}
}

bool SteamWorksHTTP::GetHandleApproxSize(HandleType_t type, void *object, unsigned int *pSize)
{
	if (type == this->typeHTTP && pSize) /* Heaven forbid we ever offer another handle. */
	{
		*pSize = sizeof(SteamWorksHTTPRequest);
		return true;
	}

	return false;
}
