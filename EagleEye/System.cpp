#include "stdafx.h"
#include "System.h"
#include "GData.h"

const uintptr_t BASE_PTR = 0xDD9D70;

System::System(Client *client)
{
	gClient = client;
}

System::~System()
{
}

void System::MainThread()
{
	printf("Loading main ptr from 0x%p...\n", BASE_PTR);
	uintptr_t mainPtr = gClient->Read<uintptr_t>((uintptr_t)BASE_PTR);
	//printf("Loaded the main pointer 0x%p...\n", mainPtr);

	if(mainPtr == 0x0) printf("Waiting for game to load completely, press F1 to exit...\n");
	while(mainPtr == 0x0 && !GetAsyncKeyState(VK_F1)){
		Sleep(1000);
		mainPtr = gClient->Read<uintptr_t>((uintptr_t)BASE_PTR);
	}

	if(mainPtr == 0x0){
		printf("World never found, exiting...\n");
		return;
	}

	printf("Final world ptr: 0x%p\n", mainPtr);
	printf("Trying to load initial snapshot...\n");
	WorldFrame frame = GData::GetData(gClient, mainPtr);

	printf("Connecting to web server...\n");
	webInterface = new Winter;

	printf("Waiting for web interface to be ready...\n");
	while(!webInterface->IsReady()){
		Sleep(200);
	}

	printf("Uploading initial map snapshot...\n");
	webInterface->UploadSnapshot(frame);

	printf("======= Map is Ready =======\n%s\n============================\n", webInterface->GetUrl().c_str());

	printf("Outputting data (and uploading), press F1 to exit.\n");

	while(!GetAsyncKeyState(VK_F1) && webInterface->IsConnected()){
		//Load data
		frame = GData::GetData(gClient, mainPtr);

		printf("\r");
		printf("Player-> x: %f y: %f z: %f dead: %u state: %u", frame.pPos.x, frame.pPos.y, frame.pPos.z, frame.player.isdead, frame.player.PlayerState);
		Sleep(1000);
	}
	printf("\nShutting down map... ");
	delete webInterface;
	printf("cleaned up!\n");
}