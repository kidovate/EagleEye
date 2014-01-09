#include "stdafx.h"
#include "Winter.h"
#include <vector>
#include <string>
#include <fstream>
#include "Tools.h"
#include <tchar.h>
#include <urlmon.h>
#pragma comment(lib, "urlmon.lib")

Winter::Winter(void)
{
	indexHtmlFilename = "1337h33x0r.html";
	GeneratePaths();
}


Winter::~Winter(void)
{
}

void Winter::GeneratePaths(){
	unsigned int result = ::GetTempPathA(0, "");
	if (result == 0) {
		printf("Could not get temporary path\n");
		return;
	}
		//throw std::runtime_error("Could not get system temp path");

	std::vector<char> tempPath(result);

	result = GetTempPathA(static_cast<unsigned int>(tempPath.size()), &tempPath[0]);
	if ((result == 0) || (result > tempPath.size())){
		printf("Could not get temporary path 2.\n");
		return;
	}
	
		//throw std::runtime_error("Could not get system temp path");

	std::string path(tempPath.begin(), tempPath.begin() + static_cast<std::size_t>(result));
	//printf("Temporary path: %s\n",  path.c_str());

	indexHtmlPath = new char[MAX_PATH];
	sprintf_s(indexHtmlPath, MAX_PATH, "%s%s", path.c_str(), indexHtmlFilename);
	//printf("Index HTML: %s\n", indexHtmlPath);

	mapjsPath = new char[MAX_PATH]; //map.js
	sprintf_s(mapjsPath, MAX_PATH, "%s%s", path.c_str(), "map.js");
	//printf("Map JS: %s\n", mapjsPath);

	playersJsonPath= new char[MAX_PATH]; //players.json
	sprintf_s(playersJsonPath, MAX_PATH, "%s%s", path.c_str(), "players.json");
	//printf("Players JSON path: %s\n", playersJsonPath);
	printf("Web interface paths generated.\n");
}

bool Winter::ExtractData() {
	CleanupFiles();
	std::ofstream locations_js_file(playersJsonPath);
	if (!locations_js_file.is_open()) {
		printf("Could not write to system...\n");
		return false;
	}

	printf("Downloading map data...\n");
	std::string uBase(baseUrl);
	std::wstring uBaseW(uBase.begin(), uBase.end());
	uBaseW.append(L"index.html");
	std::wstring indexHtmlPathW(CharToWChar(indexHtmlPath));
	HRESULT hr = URLDownloadToFile(NULL, uBaseW.c_str(), indexHtmlPathW.c_str(), 0, NULL);
	//printf("Downloaded index.html from %ls to\n %ls...\n", uBaseW.c_str(), indexHtmlPathW.c_str());

	uBaseW = std::wstring(uBase.begin(), uBase.end());
	uBaseW.append(L"map.js");
	wchar_t* indexUrlW = CharToWChar(mapjsPath);
	hr = URLDownloadToFile(NULL, uBaseW.c_str(), indexUrlW, 0, NULL);
	printf("Downloaded map.js...\n");

	locations_js_file << "[]"; //blank to start
	locations_js_file.close();
	return true;
}

void Winter::CleanupFiles(){
	remove(mapjsPath);
	remove(indexHtmlPath);
	remove(playersJsonPath);
}