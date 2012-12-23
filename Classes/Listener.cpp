#include "Listener.h"
#include "cocos2d.h"

Listener::Listener(CPhotonLib* lib)
{
	current = lib;
}

void Listener::onOperationResponse(const ExitGames::Photon::OperationResponse& operationResponse)
{
	current->onOperationResponse(operationResponse);
}

void Listener::onStatusChanged(int statusCode)
{
	current->onStatusChanged(statusCode);
}

void Listener::onEvent(const ExitGames::Photon::EventData& eventData)
{
	current->onEvent(eventData);
}

void Listener::debugReturn(PhotonPeer_DebugLevel debuglevel, const ExitGames::Common::JString& string)
{
	CCLOG("%s", string.ANSIRepresentation().cstr());
}