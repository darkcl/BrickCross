
#include "Photon_lib.h"

class Listener : public ExitGames::Photon::PhotonListener
{
public:
	Listener(CPhotonLib* lib);

private:
	void onOperationResponse(const ExitGames::Photon::OperationResponse& operationResponse);
	void onStatusChanged(int statusCode);
	void onEvent(const ExitGames::Photon::EventData& eventData);
	void debugReturn(PhotonPeer_DebugLevel debuglevel, const ExitGames::Common::JString& string);

	CPhotonLib* current;
};