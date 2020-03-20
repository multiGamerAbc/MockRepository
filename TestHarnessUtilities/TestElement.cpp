////////////////////////////////////////////////////////////////////////////////
// MyLogger.cpp - Class for holding the Dll specs for a test element.           //
// version 2.48                                                               //
// Language:    C++								                              //
// Application: Spring OOD Project 2                                          //
// Author:      Phil Maalouf, pmaalouf@syr.edu                                //
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "TestElement.h"

////////////////////////////////////////////////////////////////////////////////
// TestElement - functions
////////////////////////////////////////////////////////////////////////////////

//-------< print a TestElement >------------------------------------------------

// prints a TestElement - should use log
void TestElement::print() {
	std::cout << testDriverElement.c_str() << std::endl;
	for (std::string testedCodeElement : testedCodeElements) {
		std::cout << "    " << testedCodeElement.c_str() << std::endl;

	}
}


//-------< TestElement Test Stub >---------------------------------------------

#ifdef TEST_TESTELEMENT

int main() {
	cout << "Test Stub For TestElement" << endl;
	cout << "=========================" << endl;
	std::string testDriverSpec = "PATH/testDriver.h";
	std::vector<std::string> testedCodeSpecVector = {
		"PATH/testedCodeSpec_1.h","PATH/testedCodeSpec_2.h", 
		"PATH/testedCodeSpec_3.h"
	};
	TestElement tElem(testDriverSpec, testedCodeSpecVector);
	cout << "printing TestElement:" << endl << endl;
	tElem.print();
	cin.get();
}

#endif
