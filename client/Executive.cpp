///////////////////////////////////////////////////////////////////////
// Executive.cpp - Client application for Local Test harness         //
// version 2.48                                                      //
// Language:    C++								                     //
// Application: Spring OOD Project 2                                 //
// Author:      Phil Maalouf, pmaalouf@syr.edu                       //
///////////////////////////////////////////////////////////////////////
#include <iostream>
#include "TestHarness.h"
#include "../TestHarnessUtilities/MyLogger.h"
#include "FileManager.h"
#include <exception>
#include <string>
using namespace std;

//----< demonstrate Local Test Harness >----------------------------------------

int main()
{
	try {
		MyLogger log;  // log test results

		log.newLine();
		log.wrapEquals("Local Test Harness");
		log.newLine();

		TestHarness harness;
		FileManager manager;
		std::string testRequest;

		std::string input;
		while (true) {  // prompt for options in a loop
			manager.prompt();
			cin >> input;
			if (input[0] == 'q') {
				break;
			}
			else { // build test request and pass to test harness
				testRequest = manager.buildRequest(input[0] - '0');
				harness.processAllElements(testRequest);
			}
		}
	}
	catch (std::exception &exc) { // catch any exceptions
		cout << endl;
		cout << "Exception thrown in main:  " << exc.what() << endl << endl;
		cin.get();
		cin.get();
	}
	catch (...) {
		cout << endl;
		cout << "Non-standard exception thrown in main." << endl << endl;
		cin.get();
		cin.get();
	}

}
