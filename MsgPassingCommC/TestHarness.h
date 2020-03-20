///////////////////////////////////////////////////////////////////////
// Testharness.h   - Utility for testing applications in development //
// version 2.48                                                      //
// Language:    C++								                     //
// Application: Spring OOD Project 2                                 //
// Author:      Phil Maalouf, pmaalouf@syr.edu                       //
///////////////////////////////////////////////////////////////////////
#ifndef TESTHARNESS_H
#define TESTHARNESS_H
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include "XmlReader.h"
#include "../TestHarnessUtilities/ITestIGetTested.h"
#include "../TestHarnessUtilities/MyLogger.h"
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// TestHarness - declaraction
////////////////////////////////////////////////////////////////////////////////

class TestHarness {

public:
	// typedef generic ITest and IGetTested ctor ptrs
	typedef IGetTested*(CALLBACK* Ptr_IGetTested_Ctor_Via_Dll)();
	typedef ITest*(CALLBACK* Ptr_ITest_Ctor_Via_Dll)();

	// nested class for loading libraries, getting ctor ptrs from factories
	//   and instantiating elements
	class DllDataManipulatorForTestElement {
	public:
		DllDataManipulatorForTestElement(TestElement tE) {		
			// load test driver module	
			driverMod = LoadLibrary(tE.testDriverElement.c_str());
			// get ptr to test driver ctor
			ptrITestCtor                  
				= (Ptr_ITest_Ctor_Via_Dll)GetProcAddress(driverMod, "getCtor");
			// instantiate test driver
			ptrITest = ptrITestCtor();

			// load all tested code libraries
			for (std::string testedCodeElement : tE.testedCodeElements)  
			    testedCodeMods.push_back
			        (LoadLibrary(testedCodeElement.c_str()));
			// pack ptrs to tested code element ctors
			for (HMODULE testedCodeMod : testedCodeMods)
				ptrsIGetTestedCtors.push_back((Ptr_IGetTested_Ctor_Via_Dll)
					GetProcAddress(testedCodeMod, "getCtor"));
			// pack ptrs to tested code elements
			for (Ptr_IGetTested_Ctor_Via_Dll ptrIGetTestedCtor : 
					ptrsIGetTestedCtors)
				ptrsIGetTested.push_back(ptrIGetTestedCtor());
		}

////////////////////////////////////////////////////////////////////////////////

		// privatize, use getters

		HMODULE driverMod;							// test driver Dll module
		Ptr_ITest_Ctor_Via_Dll ptrITestCtor;		// ITest ctor pointer
		ITest* ptrITest;							// ITest*
		std::vector<HMODULE> testedCodeMods;// vector of tested code Dll modules
        // vector of IGetTested ctor pointers
		std::vector<Ptr_IGetTested_Ctor_Via_Dll> ptrsIGetTestedCtors;
		// vector of IGetTested pointers
		std::vector<IGetTested*> ptrsIGetTested;	
	};

	void processAllElements(std::string testRequest);

	void processTestElement(TestElement tE);

	void executeTestElement(DllDataManipulatorForTestElement data);

	void freeLibs(DllDataManipulatorForTestElement dllData);

	MyLogger log;

};

#endif

//--------//--------//--------//--------//--------//--------//--------//--------

