#include "stdafx.h"
#include "Winter.h"
#include "easywsclient.h"
#include <thread>         // std::thread
#include "Tools.h"
#include <sstream>
#include "JSON.h"

Winter* tmpWntr;
void Winter::handle_message(const std::string & message)
{
	Winter* winter = tmpWntr;
	if(starts_with(message, "newmapid")){
		winter->mapId = message.substr(9);
		printf("Server assigned map id %s\n", winter->mapId.c_str());// not sure why but this takes ages to print
		fflush(stdout); //tried that too and it didnt work
		winter->connectionAttemptDone = true;
	}else if(message.compare("nomapid")){
		printf("Server says we need a new map ID, requesting again...\n");
		fflush(stdout);
		winter->ws->send("reqmapid");
	}
}

void Winter::SocketThread()
{
	printf("Socket thread start...\n");
	Winter * winter = tmpWntr;
	winter->connectionAttemptDone = false;
	winter->isConnected = false;
	//stall until connected
	while(winter->ws->getReadyState() != WebSocket::OPEN && winter->ws->getReadyState() != WebSocket::CLOSED){
		winter->ws->poll();
		Sleep(200);
	}
	//winter->connectionAttemptDone = true;//wait until we get a map id
	//getchar();
	if(winter->ws->getReadyState() == WebSocket::OPEN){
		winter->ws->send("reqmapid");
		winter->isConnected = true; 
	}else{
		printf("Could not open web socket, web interface will not work!\n");
		winter->isConnected = false;
	}
    while (winter->ws->getReadyState() == WebSocket::OPEN) {
        winter->ws->poll();
		tmpWntr = winter;
        winter->ws->dispatch(handle_message);
    }
	//delete winter->ws;
	winter->isConnected = false;
	printf("Socket thread end...\n");
}

Winter::Winter(void)
{
	//open websocket, set up map
	printf("Opening websocket..\n");
	ws = WebSocket::from_url("ws://54.201.208.171:3000/gameData/websocket");
	tmpWntr = this;
	std::thread first (SocketThread);
	first.detach();
}

Winter::~Winter(void)
{
	//close websocket, tell server to delete map
	ws->close(); // this will close the ws and then get it deleted in the thread
}

void Winter::UploadSnapshot(WorldFrame frame)
{
	if(!isConnected) {
		printf("Couldn't upload snapshot: not connected.\n");
		return;
	}

	//make the snapshot object
	JSONObject root;

	//make the local player
	JSONObject lp;
	lp[L"name"] = new JSONValue("Local Player");
	lp[L"posx"] = new JSONValue(frame.pPos.x);
	lp[L"posy"] = new JSONValue(frame.pPos.y);
	lp[L"posz"] = new JSONValue(frame.pPos.z);
	lp[L"lat"] = new JSONValue(game_to_lat(frame.pPos.y));
	lp[L"lng"] = new JSONValue(game_to_lng(frame.pPos.x));
	lp[L"isdead"] = new JSONValue(frame.player.isdead);
	lp[L"playerState"] =  new JSONValue((double)frame.player.PlayerState);
	root[L"localPlayer"] = new JSONValue(lp);

	//players array
	JSONArray players;
	//foreach etc
	//add it
	root[L"players"] = new JSONValue(players);

	//stringify
	JSONValue f(root);
	std::string json = ws2s(f.Stringify());
	//printf("JSON: %s\n", json);
	//upload to server
	ws->send(std::string("setsnapshot:")+json);
}

bool Winter::IsReady(){
	return connectionAttemptDone;
}

bool Winter::IsConnected(){
	return isConnected;
}

std::string Winter::GetUrl(){
	return std::string("http://54.201.208.171:3000/map/")+mapId;
}