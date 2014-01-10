#pragma once
//Stores handle to game process
class Client
{
private:
	HANDLE gHandle;
public:
	Client(void);
	~Client(void);

	//Returns if game was found or not. If blockUntilFound it will display a message and wait for the game to open
	bool LoadGame(bool blockUntilFound);

	//Read from the memory
	template <class T>
	T Read(uintptr_t addr){
		T val;
		ReadProcessMemory(gHandle, (void*)addr, &val, sizeof(T), 0);
		return val;
	}
};
