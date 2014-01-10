#pragma once
#include "easywsclient.h"
#include "GData.h"

using easywsclient::WebSocket;
class Winter
{
private:
	std::string mapId;
	static void SocketThread();
	static void handle_message(const std::string & message);
	bool connectionAttemptDone;
	bool isConnected; //thread safe
public:
	WebSocket::pointer ws;
	Winter(void);
	~Winter(void);
	void UploadSnapshot(WorldFrame frame);
	bool IsReady();
	bool IsConnected();
	std::string GetUrl();
};
