// TestApp2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../client/TestHarness.h"

void runTests() {
	HMODULE driverMod;
	driverMod = LoadLibrary("../SaveFiles/MathTestDriver.dll");
	typedef ITest*(CALLBACK* Ptr_ITest_Ctor_Via_Dll)();
	Ptr_ITest_Ctor_Via_Dll ptrITestCtor;
	ITest* ptrITest;
	ptrITestCtor
		= (Ptr_ITest_Ctor_Via_Dll)GetProcAddress(driverMod, "getCtor");
	ptrITest = ptrITestCtor();

	HMODULE testedCodeMod;
	testedCodeMod = LoadLibrary("../SaveFiles/MathImpl1.dll");
	typedef IGetTested*(CALLBACK* Ptr_IGetTested_Ctor_Via_Dll)();
	Ptr_IGetTested_Ctor_Via_Dll ptrIGetTestedCtor;
	IGetTested* ptrIGetTested;
	ptrIGetTestedCtor = (Ptr_IGetTested_Ctor_Via_Dll)
		GetProcAddress(testedCodeMod, "getCtor");
	ptrIGetTested = ptrIGetTestedCtor();

	(*ptrITest).processTestedCodeElement(ptrIGetTested);
}

int main()
{
	cout << "IN TEST APP beta 2" << endl;
	runTests();
	cin.get();
    return 0;
}

