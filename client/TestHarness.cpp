///////////////////////////////////////////////////////////////////////
// Testharness.cpp - Utility for testing applications in development //
// version 2.48                                                      //
// Language:    C++								                     //
// Application: Spring OOD Project 2                                 //
// Author:      Phil Maalouf, pmaalouf@syr.edu                       //
///////////////////////////////////////////////////////////////////////

#include "TestHarness.h"

////////////////////////////////////////////////////////////////////////////////
// TestHarness - functions
////////////////////////////////////////////////////////////////////////////////


//----< parse test reqeust and process all elements >---------------------------

void TestHarness::processAllElements(std::string testRequest) {
	try {
		char testElement = 'A';  // max: 26
		XmlReader rdr(testRequest);
		std::vector<TestElement> vecOfTestElement 
			= rdr.parseXmlIntoVectorOfTestElements();

		for (TestElement tElem : vecOfTestElement) {
			ostringstream msg;
			msg << "Processing TestElement " << testElement++;

			log.newLine();
			log.wrapEquals("");
			log.wrapEquals(msg.str());
			log.wrapEquals("");

			processTestElement(tElem);
		}
	}
	catch (std::exception &exc) {
		cout << endl;
		cout << "Exception thrown in processAllElements:  " 
			<< exc.what() << endl << endl;
		throw exc;
	}
	catch (...) {
		cout << endl;
		cout << "Non-standard exception thrown in processAllElements."
			<< endl << endl;
		throw "Some Non-standard exception";
	}
}

//----< load libraries and excecute test element >------------------------------

void TestHarness::processTestElement(TestElement tE) {
	try{

		DllDataManipulatorForTestElement data(tE);

		executeTestElement(data);

		freeLibs(data);

	}
	catch (std::exception &exc) {
		cout << endl;
		cout << "Exception thrown in processTestElements:  "
			<< exc.what() << endl << endl;
		throw exc;
	}
	catch (...) {
		cout << endl;
		cout << "Non-standard exception thrown in processTestElements."
			<< endl << endl;
		throw "Some Non-standard exception";
	}
}

//----< pass each tested code element to test driver, log results >-------------

void TestHarness::executeTestElement(DllDataManipulatorForTestElement data) {
	int testedCodeElementCount = 0;
	for (IGetTested* ptrTestedCode : data.ptrsIGetTested) {
		try{
			string procElemMsg = "Processing TestedCodeElement " 
				+ to_string(++testedCodeElementCount);
		
			log.newLine();
			log.wrapSlashRight(procElemMsg);
			
								             
			bool testResult      // runs test function on tested code element
				= data.ptrITest->processTestedCodeElement(ptrTestedCode);

			// Single responsibility, do this in MyLogger
			std::string timeStr = log.time().str().substr(0,17);

			string testResultMsg = "Test Result for testedCodeElement "
				+ to_string(testedCodeElementCount) +
				":  " + to_string(testResult);

			string timeDateStamp = "Time-Date Stamp for testedCodeElement "
				+ to_string(testedCodeElementCount) +
				":   " + timeStr;

			log.newLine();
			log.wrapDashRightWithSlash(testResultMsg);
			log.wrapDashRightWithSlash(timeDateStamp);
			log.wrapDash("");
			log.newLine();
		}
		catch (std::exception &exc) {
			cout << endl;
			cout << "Exception thrown in executeTestElement:  "
				<< exc.what() << endl << endl;
			throw exc;
		}
		catch (...) {
			cout << endl;
			cout << "Non-standard exception thrown in executeTestElement." 
				<< endl << endl;
			throw "Some Non-standard exception";
		}
	}
}


//----< free libraries associated with a test element >-------------------------

void TestHarness::freeLibs(DllDataManipulatorForTestElement dllData) {
	FreeLibrary(dllData.driverMod);
	for (HMODULE testedCodeMod : dllData.testedCodeMods) {
		FreeLibrary(testedCodeMod);
	}
}

//----< test stub >-------------------------------------------------------------
#ifdef TEST_TESTHARNESS

#include "FileManager.h"

int main()
{
	try {
		MyLogger log;

		log.newLine();
		log.wrapEquals("Test Stub for TestHarness");
		log.wrapEquals("");
		log.newLine();

		TestHarness harness;
		FileManager manager;
		std::string testRequest;

		log.wrapDash("Press enter to process Math test element");
		log.newLine();
		cin.get();
		testRequest = manager.buildRequest(1);
		harness.processAllElements(testRequest);

		log.wrapDash("Press enter to process Logic and Car test elements");
		log.newLine();
		cin.get();
		testRequest = manager.buildRequest(6);
		harness.processAllElements(testRequest);

		log.wrapDash("Press enter to process Math, Logic, and Car test elements");
		log.newLine();
		cin.get();
		testRequest = manager.buildRequest();
		harness.processAllElements(testRequest);

		log.wrapDash("End TestHarness test stub");
		log.newLine();
		cin.get();
	}
	catch (std::exception &exc) {
		cout << endl;
		cout << "Exception thrown in test stub:  " << exc.what() << endl << endl;
		cin.get();
		cin.get();
	}
	catch (...) {
		cout << endl;
		cout << "Non-standard exception thrown in test stub." << endl << endl;
		cin.get();
		cin.get();
	}

}

#endif