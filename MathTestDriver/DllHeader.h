////////////////////////////////////////////////////////////////////////////////
// DllHeader.h - A file for MathTestDriver DLL Functionality                  //
// version 2.48                                                               //
// Language:    C++								                              //
// Application: Spring OOD Project 2                                          //
// Author:      Phil Maalouf, pmaalouf@syr.edu                                //
////////////////////////////////////////////////////////////////////////////////
#ifndef DLLHEADER_H
#define DLLHEADER_H
#include "stdafx.h"

////////////////////////////////////////////////////////////////////////////////
// DLL Functionality Code
////////////////////////////////////////////////////////////////////////////////

#ifdef MATHTESTDRIVER_EXPORTS
#define TEST_API __declspec(dllexport)
#endif

BOOL APIENTRY DllMain(
	HANDLE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

#endif