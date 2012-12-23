#ifndef __CPHOTON_LIB_H
#define __CPHOTON_LIB_H

#include "LitePeer.h"
#include "MultiPlayConstant.h"

class Listener;
class MultiPlayerBrickGameScene;

class CPhotonLib
{
public:
	CPhotonLib(void);
	~CPhotonLib(void);

	void initLib(ExitGames::Photon::PhotonListener* PhotonListener, MultiPlayerBrickGameScene* sample);
	void run(void); // return true if still in game
	void gameLeft(void);
	void sendData(unsigned char eventCode);

	// Sample internal states
	enum States
	{
		statePhotonPeerCreated,
		stateConnecting,
		stateConnected,
		stateJoining,
		stateErrorJoining,
		stateJoined,
		stateLeaving,
		stateErrorLeaving,
		stateLeft,
		stateErrorConnecting,
		stateReceiving,
		stateDisconnecting,
		stateDisconnected,
	};

	States getState(void);
	void setState(States state);
	void setPhotonType(unsigned char type);
protected:
	void createConnection();
	void closeConnection();
private:
	friend class Listener;

	void onOperationResponse(const ExitGames::Photon::OperationResponse& operationResponse);
	void onStatusChanged(int statusCode);
	void onEvent(const ExitGames::Photon::EventData& eventData);

	static ExitGames::Common::Hashtable* createTestHashtable();

	States mCurrentState;
	ExitGames::Photon::LitePeer* mpPeer;
	MultiPlayerBrickGameScene* mpSample;
	bool mIsInGame;
	unsigned char mPhotonType;
};

#endif