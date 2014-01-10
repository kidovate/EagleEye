#pragma once
#include "stdafx.h"
#include "Client.h"
#include "Winter.h"
//Main system manager for the map
class System
{
private:
	Client* gClient;
public:
	System(Client *client);
	~System();
	Winter* webInterface;

	//The main thread for the program.. Process will exit when it returns
	void MainThread();
};
