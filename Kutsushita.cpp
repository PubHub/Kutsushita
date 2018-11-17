#define _CRT_SECURE_NO_WARNINGS
#define SOURCE_IN_CP932
#include "XAntiDebug/XAntiDebug.h"
#include <iostream>
#include <windows.h>
#include <WinUser.h>
#include <WinBase.h>
#include <tchar.h>
#include <urlmon.h>
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib,"wininet.lib")
#pragma comment(lib, "Advapi32.lib")
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <WinInet.h>
#include <iomanip>
#include <cstdio>
#include <fstream>
#include <urlmon.h>
#include <io.h>
#include <shellapi.h>
#pragma comment(lib, "Urlmon.lib")
#pragma comment(lib, "Shell32.lib")
#include <windows.h>
#include <locale.h>
#include <thread>
#include <Psapi.h>
using namespace std;
int intExit() { int a = 1, b = 0; return a / b; }
bool IsHTTPDebuggerInstalled()
{
	LPVOID drivers[2048];
	DWORD cbNeeded;
	int cDrivers, i;

	if (EnumDeviceDrivers(drivers, sizeof(drivers), &cbNeeded) && cbNeeded < sizeof(drivers))
	{
		TCHAR szDriver[2048];

		cDrivers = cbNeeded / sizeof(drivers[0]);

		for (i = 0; i < cDrivers; i++)
		{
			if (GetDeviceDriverBaseName(drivers[i], szDriver, sizeof(szDriver) / sizeof(szDriver[0])))
			{
				std::string strDriver = szDriver;
				if (strDriver.find(("HttpDebug")) != std::string::npos)
					return true;
			}
		}
	}
	return false;
}
void GuardTh()
{
	for (;;)
	{
		XAD_STATUS status;
		XAntiDebug antiDbg(GetModuleHandle(NULL), FLAG_DETECT_DEBUGGER | FLAG_DETECT_HARDWAREBREAKPOINT);

		status = antiDbg.XAD_Initialize();
		if (status != XAD_OK)
		{
			__asm int 0;
		}
		if (antiDbg.XAD_ExecuteDetect())
		{
			__asm int 0;
		}
		LPCTSTR FiddlerClass = "WindowsForms10.Window.8.app.0.2bf8098_r6_ad1";
		LPCTSTR FiddlerName = "Progress Telerik Fiddler Web Debugger";
		LPCTSTR HTTPDebuggerClass = "XTPMainFrame";
		LPCTSTR HTTPDebuggerName = "HTTP Debugger Pro v8.20";
		HWND FindNFinddler = FindWindow(NULL, FiddlerName);
		HWND FindCFiddler = FindWindow(FiddlerClass, NULL);
		HWND FindNHTTPDebugger = FindWindow(NULL, HTTPDebuggerName);
		HWND FindCHTTPDebugger = FindWindow(HTTPDebuggerClass, NULL);
		if (FindNFinddler || FindCFiddler || FindNHTTPDebugger || FindCHTTPDebugger || IsHTTPDebuggerInstalled())
		{
			MessageBox(NULL, "Gei ga mitsukarimashita!", "Kutsushita", MB_OK);
			__asm int 0;
		}
	}
}
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	WaitForSingleObject(CreateThread(nullptr, 0, LPTHREAD_START_ROUTINE(GuardTh), nullptr, 0, nullptr), INFINITE);
	intExit();
}