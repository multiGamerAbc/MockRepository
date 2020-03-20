// passed std:string to LoadLibrary

#include <iostream>
#include "TxGTxPair.h"
#include "XmlReader.h"
#include <windows.h>
#include "imagehlp.h"
#include <vector>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// TestElement class         --- Declaration and Definition
////////////////////////////////////////////////////////////////////////////////



class LoadBuildAndTest {
public:

};

////////////////////////////////////////////////////////////////////////////////
// TestHarness class         --- Declaration and Definition
////////////////////////////////////////////////////////////////////////////////

class TestHarness {
public:

	typedef GTx*(CALLBACK* Ptr_GTx_Ctor_Via_Dll)();
	typedef Tx*(CALLBACK* Ptr_Tx_Ctor_Via_Dll)();



	class DllDataManipulatorForTestElement {
	public:
		DllDataManipulatorForTestElement(TestElement tE) {
			driverMod = LoadLibrary((LPCWSTR)tE.testDriverElement.c_str());  // ## Why did I have to typecast this?
			ptrTxCtor = (Ptr_Tx_Ctor_Via_Dll)GetProcAddress(driverMod, "getCtor");
			ptrTx = ptrTxCtor();

			for (std::string testedCodeElement : tE.testedCodeElements)
				testedCodeMods.push_back(LoadLibrary((LPCWSTR)testedCodeElement.c_str())); // ## Why did I have to typecast this?
			for (HMODULE testedCodeMod : testedCodeMods)
				ptrsGTxCtors.push_back((Ptr_GTx_Ctor_Via_Dll)GetProcAddress(testedCodeMod, "getCtor"));
			for (Ptr_GTx_Ctor_Via_Dll ptrGTxCtor : ptrsGTxCtors)
				ptrsGTx.push_back(ptrGTxCtor());

		}
		HMODULE driverMod;
		Ptr_Tx_Ctor_Via_Dll ptrTxCtor;
		Tx* ptrTx;
		std::vector<HMODULE> testedCodeMods;
		std::vector<Ptr_GTx_Ctor_Via_Dll> ptrsGTxCtors;
		std::vector<GTx*> ptrsGTx;
	};

	void populateTestElement1(TestElement* pTE, std::string xmlStr) {
		std::string houseAFrameDllSpec = "debug/HouseAFrame.dll", houseRanchDllSpec = "debug/HouseRanch.dll";
		std::string houseIglooDllSpec = "debug/HouseIgloo.dll";

		std::string houseTestDriverDllSpec = "debug/HouseTestDriver.dll";

		std::vector<std::string> v{ houseAFrameDllSpec, houseRanchDllSpec, houseIglooDllSpec };
		pTE->testDriverElement = houseTestDriverDllSpec;
		pTE->testedCodeElements = v;
	}

	void populateTestElement2(TestElement* pTE, std::string xmlStr) {
		std::string farmCornDllSpec = "debug/FarmCorn.dll";

		std::string farmTestDriverDllSpec = "debug/FarmTestDriver.dll";

		std::vector<std::string> v{ farmCornDllSpec };
		pTE->testDriverElement = farmTestDriverDllSpec;
		pTE->testedCodeElements = v;
	}

	std::vector<TestElement> parseXmlIntoVectorOfTestElements(TestElement* pTE, std::string xmlStr) {
		std::vector<TestElement> r;
		int numElements = 2;

	}

	void executeTestElement(DllDataManipulatorForTestElement data) {
		for (GTx* ptrTestedCode : data.ptrsGTx) {
			data.ptrTx->test(ptrTestedCode);
		}
	}

	void freeLibs(DllDataManipulatorForTestElement dllData) {  // Test This Function

		FreeLibrary(dllData.driverMod);
		for (HMODULE testedCodeMod : dllData.testedCodeMods) {
			FreeLibrary(testedCodeMod);
		}
	}

	void processTestElement(TestElement tE) {
		cout << "Entering processTestElement" << endl;

		DllDataManipulatorForTestElement data(tE);

		executeTestElement(data);

		freeLibs(data);

		cout << endl << "Leaving processTestElement" << endl;
	}

	void processAllElements(std::string singleXmlString) {
		// should take a single XML string, parse it, and create a vector of
		//  TestElements.  Should iterate over vector and process each
		//  TestElement.
		//TestElement testElementForHouse;
		//TestElement testElementForFarm;

		//populateTestElement1(&testElementForHouse, "xmlStrLiteral");
		//populateTestElement2(&testElementForFarm, "xmlStrLiteral");

		std::string str = "<TestElement>"
			"<TestDriverElem>debug/HouseTestDriver.dll</TestDriverElem>"
			"<TestedCodeElem>debug/HouseAFrame.dll</TestedCodeElem>"
			"<TestedCodeElem>debug/HouseRanch.dll</TestedCodeElem>"
			"<TestedCodeElem>debug/HouseIgloo.dll</TestedCodeElem>"
			"</TestElement>"
			"<TestElement>"
			"<TestDriverElem>debug/FarmTestDriver.dll</TestDriverElem>"
			"<TestedCodeElem>debug/FarmCorn.dll</TestedCodeElem>"
			"</TestElement>"
			;

		XmlReader rdr(str);
		std::vector<TestElement> vec = rdr.parseXmlIntoVectorOfTestElements();

		cout << endl << "***" << endl;
		for (TestElement tE : vec) {
			processTestElement(tE);
			cout << endl << "***" << endl;
		}
	}

	void processAllElements2(std::string singleXmlString) {
		// should take a single XML string, parse it, and create a vector of
		//  TestElements.  Should iterate over vector and process each
		//  TestElement.
		TestElement testElementForHouse;
		TestElement testElementForFarm;

		populateTestElement1(&testElementForHouse, "xmlStrLiteral");
		populateTestElement2(&testElementForFarm, "xmlStrLiteral");

		processTestElement(testElementForHouse);
		cout << endl << "***" << endl;
		processTestElement(testElementForFarm);
	}

};

//--------//--------//--------//--------//--------//--------//--------//--------





////////////////////////////////////////////////////////////////////////////////
// #########################################################################  //
////////////////////////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////////////////////////
// Old #includes
////////////////////////////////////////////////////////////////////////////////

//#include "../Math/Math.h"
//#include "../MathDriver/MathDriver.h"
//#include "../MathDriver/D1.h"