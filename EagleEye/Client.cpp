#include "stdafx.h"
#include "Client.h"
#include "Tools.h"

Client::Client(void)
{
}

Client::~Client(void)
{
	CloseHandle(gHandle);
}

bool Client::LoadGame(bool block){
	HANDLE h;
	DWORD pid = FindProcessId(TEXT(GAME_NAME));
	if(pid==NULL){
		if(block)
			printf("Game is not running, will continue searching, press HOME to exit.\n");
		else{
			printf("Could not find the game, exiting...");
			return false;
		}
	}

	while(pid == NULL && !GetAsyncKeyState(VK_HOME)){
		pid = FindProcessId(TEXT(GAME_NAME));
		Sleep(1000);
	}
	if(pid==NULL){
		printf("Never found the game, exiting...\n");
		return false;
	}
	printf("Found game, PID: %lu, opening handle...\n", pid);

	if (!(h = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid)))
	{
		printf("Failed to open a handle on the game process... Press enter to continue.\n");
		getchar();
		return false;
	}

	gHandle = h;
	printf("Opened handle to game...\n");
	return true;
}