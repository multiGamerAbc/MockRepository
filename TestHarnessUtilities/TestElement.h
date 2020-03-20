////////////////////////////////////////////////////////////////////////////////
// MyLogger.h - Class for holding the Dll specs for a test element.             //
// version 2.48                                                               //
// Language:    C++								                              //
// Application: Spring OOD Project 2                                          //
// Author:      Phil Maalouf, pmaalouf@syr.edu                                //
////////////////////////////////////////////////////////////////////////////////
#ifndef TESTELEMENT_H
#define TESTELEMENT_H
#include <iostream>
#include <vector>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// TestElement -- holds DLL specs for test driver and tested code elements
////////////////////////////////////////////////////////////////////////////////

class TestElement {

public:
	TestElement();
	TestElement(std::string tDriverElem, std::vector<std::string> tCodeElems) :
		testDriverElement(tDriverElem), testedCodeElements(tCodeElems) {}
	TestElement prefixPath(std::string prefix) {
		std::string retValTestDriverElement;
		std::vector<std::string> retValTestedCodeElements;
		retValTestDriverElement = prefix + testDriverElement;
		for (std::string testedCodeElement : testedCodeElements)
			retValTestedCodeElements.push_back(prefix + testedCodeElement);
		TestElement retVal(retValTestDriverElement, retValTestedCodeElements);
		return retVal;
	}
	TestElement addFileExtensions(std::string extension) {
		std::string retValTestDriverElement;
		std::vector<std::string> retValTestedCodeElements;
		retValTestDriverElement = testDriverElement + extension;
		for (std::string testedCodeElement : testedCodeElements)
			retValTestedCodeElements.push_back(testedCodeElement + extension);
		TestElement retVal(retValTestDriverElement, retValTestedCodeElements);
		return retVal;
	}
	std::string toString() {
		std::string str = "<TestElement>"
			"<TestDriverElem>MathTestDriver.dll</TestDriverElem>"
			"<TestedCodeElem>MathImpl1.dll</TestedCodeElem>"
			"<TestedCodeElem>MathImpl2.dll</TestedCodeElem>"
			"</TestElement>";
		std::string retVal = "<TestElement>";
		retVal += "<TestDriverElem>" + testDriverElement + "</TestDriverElem>";
		for (std::string testedCodeElement : testedCodeElements)
			retVal += "<TestedCodeElem>" + testedCodeElement + "</TestedCodeElem>";
		retVal += "</TestElement>";
		return retVal;
	}
	void print();

	// Should be privatized, getters added
	std::string testDriverElement;
	std::vector<std::string> testedCodeElements;

};

#endif

//--------//--------//--------//--------//--------//--------//--------//--------
