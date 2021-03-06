// Key Generator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <string>
#include <atlbase.h>
#include <atlconv.h>

#ifndef UNICODE
    #define _tstring string
    #define to_tstring to_string
#else
    #define _tstring wstring
    #define to_tstring to_wstring
#endif
using namespace std;
#define REGISTRY_PATH _T("SOFTWARE\\Enzo Tech Peso-Net Timer")


char rightRotate(unsigned char n, int nTimes)
{
	for (int i = 0; i < nTimes; i++)
	{
		n = (n >> 1) | (n << 7);
	}
	return n;
}

char leftRotate(unsigned char n, int nTimes)
{
	for (int i = 0; i < nTimes; i++)
	{
		n = (n << 1) | (n >> 7);
	}
	return n;
}

string convertStringToHexString(string s)
{
	string sRes;
	size_t len = s.length();
	BYTE byTemp;
	char szTemp[3];
	for (size_t i = 0; i < len; i++)
	{
		byTemp = s[i];
		sprintf_s(szTemp,sizeof(szTemp), "%X", byTemp);
		sRes.append(szTemp);
	}
	return sRes;
}

byte char2int(char input)
{
	if (input >= '0' && input <= '9')
		return input - '0';
	if (input >= 'A' && input <= 'F')
		return input - 'A' + 10;
	if (input >= 'a' && input <= 'f')
		return input - 'a' + 10;
	throw std::invalid_argument("Invalid input string");
}

string convertHexStringToString(string s)
{
	string sRes;
	size_t len = s.length();
	BYTE byTemp;

	for (size_t i = 0; i < len; i+=2)
	{
		byTemp = char2int(s[i]) << 4;
		byTemp |= char2int(s[i+1]);
		sRes.push_back(byTemp);
	}
	return sRes;
}

string encrypt(string s)
{
	string sKey = "EnzoTechComputerSolutions";
	size_t nLenKey = sKey.length();
	size_t nLen = s.length();
	string sNew = "";

	for (int i = 0; i < nLenKey; i++)
	{
		sKey[i] = rightRotate(sKey[i], nLenKey);
	}

	for (size_t i = 0; i < nLen; i++)
	{
		s[i] = rightRotate(s[i], nLen);
	}
	string sRet = sKey + s;
	return convertStringToHexString(sRet);
}

string decrypt(string s)
{
	size_t nLen = s.length();
	if ((nLen % 2))
		return s;

	s = convertHexStringToString(s);

	nLen = s.length();
	string sKey = "EnzoTechComputerSolutions";
	size_t nLenKey = sKey.length();
	string sNew = "";
	string sGetKey = s.substr(0, nLenKey);

	for (int i = 0; i < nLenKey; i++)
	{
		sGetKey[i] = leftRotate(sGetKey[i], nLenKey);
	}
	if (sGetKey.compare(sKey) != 0)
		return s;

	sNew = s.substr(nLenKey, nLen);
	nLen = sNew.size();
	for (size_t i = 0; i < nLen; i++)
	{
		sNew[i] = leftRotate(sNew[i], nLen);
	}
	return sNew;
}

int main()
{
    SYSTEMTIME sysTime;
    ULONGLONG ulldays;
    _tstring sDate;
    DWORD dwError = 0;
    HKEY hKey;
    memset(&sysTime, 0, sizeof(sysTime));
    
    GetLocalTime(&sysTime);
    ulldays = ((sysTime.wYear * 12 * 30) + sysTime.wMonth * 30 + sysTime.wDay);

    sDate = to_tstring(ulldays);

    dwError = RegOpenKeyEx(HKEY_LOCAL_MACHINE, REGISTRY_PATH, 0, KEY_ALL_ACCESS, &hKey);
    if (dwError != ERROR_SUCCESS)
    {
        if (dwError == ERROR_ACCESS_DENIED)
            MessageBox(NULL, _T("Access denied. Please run the program as Administrator"), _T("Enzo Tech Peso-Net Timer"), MB_ICONERROR);
        else
            MessageBox(NULL, _T("Contact Enzo Tech Computer Solutions for activation of the Tool."), _T("Enzo Tech Peso-Net Timer"), MB_ICONERROR);
        return 0;
    }
	_tstring sResult = CA2W(encrypt(CW2A(sDate.c_str(),CP_UTF8).m_psz).c_str(), CP_UTF8).m_psz;
    dwError = RegSetValueEx(hKey, _T("date"), 0, REG_SZ, (LPBYTE)CA2W(encrypt(CW2A(sDate.c_str(), CP_UTF8).m_psz).c_str(), CP_UTF8).m_psz, sizeof(TCHAR) * sResult.length());
    if (dwError != ERROR_SUCCESS)
    {
        return 0;
    }
    //std::cout << "Hello World!\n";

    RegCloseKey(hKey);
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
