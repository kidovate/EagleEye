#pragma once
#include <string>

namespace{
	static const char* baseUrl = "http://54.201.208.171:8000/";
}
class Winter
{
private:
	char*indexHtmlFilename;
	void GeneratePaths();
public:
	Winter(void);
	~Winter(void);
	bool ExtractData();
	void CleanupFiles();
	char* indexHtmlPath;
	char* mapjsPath;
	char* playersJsonPath;
};

