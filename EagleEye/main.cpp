// EagleEye.cpp : Defines the entry point for the console application.
//
#define WIN32_LEAN_AND_MEAN
#include "stdafx.h"
#include "System.h"
#include "Console.h"
#include "Client.h"

bool shutDown = false;
bool mainThreadLaunched = false;
bool hasShutDown = false;
HMODULE ourHandle;

void ForceUnloadDLL()
{
	FreeLibraryAndExitThread(ourHandle, 0);
}

void ShutDown(){
	if (hasShutDown) return;
	hasShutDown = true;
	shutDown = true;
	ForceUnloadDLL();
	CloseHandle(ourHandle);
}

DWORD WINAPI MainThread(LPVOID lpParam){
	mainThreadLaunched = true;
	CConsole *console = new CConsole();
	Client* client = new Client;
	if(client->LoadGame(true)){
		System* system = new System(client);
		system->MainThread();
		delete system;
	}
	delete client;
	delete console;
	ShutDown();
	return NULL;
}

BOOL APIENTRY DllMain(HINSTANCE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		if (mainThreadLaunched || shutDown || hasShutDown) break;
		ourHandle = hModule;
		//Launch the main system
		DWORD ThreadId;
		CreateThread(NULL, 0, MainThread, NULL, 0, &ThreadId);
		break;
	case DLL_PROCESS_DETACH:
		ShutDown();
		break;

	case DLL_THREAD_ATTACH:
		break;

	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

