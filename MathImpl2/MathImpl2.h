////////////////////////////////////////////////////////////////////////////////
// MathImpl2.h - Specialization of Math class                                 //
// version 2.48                                                               //
// Language:    C++								                              //
// Application: Spring OOD Project 2                                          //
// Author:      Phil Maalouf, pmaalouf@syr.edu                                //
////////////////////////////////////////////////////////////////////////////////
#ifndef MATH_IMPL_2_H
#define MATH_IMPL_2_H
#include "stdafx.h"
#include <iostream>
#include "../TestHarnessUtilities/ITestIGetTested.h"
#include "../Math/Math.h"
//#include "DllHeader.h" // this caused an issue

////////////////////////////////////////////////////////////////////////////////
// DLL Functionality Code
////////////////////////////////////////////////////////////////////////////////

#ifdef MATHIMPL2_EXPORTS
#define TEST_API __declspec(dllexport)
#endif


//----< DllMain >---------------------------------------------------------------
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