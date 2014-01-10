#pragma once
#include "stdafx.h"
#include <TlHelp32.h>
#include <string>
#include <locale>
#include <codecvt>

namespace
{
	DWORD FindProcessId(const wchar_t *processname)
	{
		HANDLE hProcessSnap;
		PROCESSENTRY32 pe32;
		DWORD result = NULL;

		// Take a snapshot of all processes in the system.
		hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (INVALID_HANDLE_VALUE == hProcessSnap) return(FALSE);

		pe32.dwSize = sizeof(PROCESSENTRY32);

		// Retrieve information about the first process,
		// and exit if unsuccessful
		if (!Process32First(hProcessSnap, &pe32))
		{
			CloseHandle(hProcessSnap);
			printf("!!! Failed to gather information on system processes! \n");
			return(NULL);
		}

		do
		{
			if (0 == wcscmp(processname, pe32.szExeFile))
			{
				result = pe32.th32ProcessID;
				break;
			}
		} while (Process32Next(hProcessSnap, &pe32));

		CloseHandle(hProcessSnap);

		return result;
	}

	std::string XOREncryption(std::string str, int key)
	{
		std::string enc("");
		for (unsigned int i(0); i < str.length(); i++) // iterates through the string to encrypt
			enc += str[i] ^ key; // ^ - XOR operator in C++
		return enc;
	}
	std::string XORDecryption(std::string str, int key)
	{
		std::string dec("");
		for (unsigned int i(0); i < str.length(); i++)// iterates through the string to decrypt
			dec += str[i] ^ key; // ^ - XOR operator in C++
		return dec;
	}
	std::string ReplaceString(std::string subject, const std::string& search,
		const std::string& replace) {
			size_t pos = 0;
			while ((pos = subject.find(search, pos)) != std::string::npos) {
				subject.replace(pos, search.length(), replace);
				pos += replace.length();
			}
			return subject;
	}
	wchar_t* CharToWChar(char* input){
		size_t wn = mbsrtowcs(NULL, ((const char**)&input), 0, NULL);
		wchar_t*buff = new wchar_t[wn+1]();
		wn = mbsrtowcs(buff, ((const char**)&input), wn+1, NULL);
		return buff;
	}
	bool starts_with(const std::string& s1, const std::string& s2) {
		return s2.size() <= s1.size() && s1.compare(0, s2.size(), s2) == 0;
	}

	
	float game_to_lng(float x) {
		return (float)(0.00260491 + 0.000999703*x);
	}
	float game_to_lat(float y) {
		return (float)(15.3478 - 0.000998826*y);
	}

	std::wstring s2ws(const std::string& str)
	{
		typedef std::codecvt_utf8<wchar_t> convert_typeX;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.from_bytes(str);
	}

	std::string ws2s(const std::wstring& wstr)
	{
		typedef std::codecvt_utf8<wchar_t> convert_typeX;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.to_bytes(wstr);
	}
}