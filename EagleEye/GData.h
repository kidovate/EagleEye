#pragma once
#include "stdafx.h"
#include "Client.h"

//Game data structs, hopefully not detected

class World //base + 1860
{
public:
	char pad[1860];
	uintptr_t objTPtr; //0x0744
	char pad1[2112];
	uintptr_t nearT; //0x0F88 CPlayer**
	__int32 nearTSize; //0x0F8C
	char pad2[160];
	uintptr_t farT; //0x1030 CPlayer**
	__int32 farTSize; //0x1034
	char pad3[496];
	uintptr_t unknownT; //0x1228 CPlayer**
	__int32 unknownTSize; //0x122C
	char pad4[864];
	uintptr_t localEntity; //0x1590 CEntity*
};

class Entity{ //Pad 0x004 before
public:
	uintptr_t playerPtr; //0x004 Cplayer*
};

class Player{ //Pad 0x20 before
public:
	uintptr_t pos; //0x0020 CPlayerPosition*
	char pad[76];
	uintptr_t objClass; //0x0070 CObjectClass*
	char pad1[0x264 - 0x74];
	bool isdead; //0x264
	char pad2[0x7C8 - 0x268];
	int playerId; //0x7C8
	char pad3[0x12AC - 0x7CC];
	int PlayerState; //0x12AC
};

class Position{ //0x0028 pad before
public:
	FLOAT x;
	FLOAT z;
	FLOAT y;
};

//All data recorded for a moment
class WorldFrame {
public:
	World world;
	Entity localEnt;
	Player player;
	Position pPos;
};

class GData
{
public:
	static WorldFrame GetData(Client * client, uintptr_t base){
		WorldFrame frame;

		//Resolve the world
		World world = client->Read<World>(base);
		frame.world = world;

		//Resolve the local entity
		Entity localEnt = client->Read<Entity>(world.localEntity+0x004);
		frame.localEnt = localEnt;

		//Resolve the local player
		Player player = client->Read<Player>(localEnt.playerPtr+0x20);
		frame.player = player;

		//Resolve the player pos
		Position playerPos = client->Read<Position>(player.pos+0x0028);
		frame.pPos = playerPos;

		//Try loading local entity manually
		//frame.localEnt = client->Read<uintptr_t>(base+

		return frame;
	}
};
