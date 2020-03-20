///////////////////////////////////////////////////////////////////////
// FileManager.h   - Dll spec "server" for Local Test harness        //
// version 2.48                                                      //
// Language:    C++								                     //
// Application: Spring OOD Project 2                                 //
// Author:      Phil Maalouf, pmaalouf@syr.edu                       //
///////////////////////////////////////////////////////////////////////
#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <iostream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// FileManager - declaraction
////////////////////////////////////////////////////////////////////////////////

class FileManager {
public:
	std::string buildRequest(int mode = 0);
	void prompt();

private:

	std::string MathTestRequest =
		"<TestElement>"
		"<TestDriverElem>debug/MathTestDriver.dll</TestDriverElem>"
		"<TestedCodeElem>debug/MathImpl1.dll</TestedCodeElem>"
		"<TestedCodeElem>debug/MathImpl2.dll</TestedCodeElem>"
		"</TestElement>";

	std::string LogicTestRequest =
		"<TestElement>"
		"<TestDriverElem>debug/LogicTestDriver.dll</TestDriverElem>"
		"<TestedCodeElem>debug/LogicImpl1.dll</TestedCodeElem>"
		"<TestedCodeElem>debug/LogicImpl2.dll</TestedCodeElem>"
		"<TestedCodeElem>debug/LogicImpl3.dll</TestedCodeElem>"
		"</TestElement>";

	std::string CarTestRequest =
		"<TestElement>"
		"<TestDriverElem>debug/CarTestDriver.dll</TestDriverElem>"
		"<TestedCodeElem>debug/CarImpl1.dll</TestedCodeElem>"
		"<TestedCodeElem>debug/CarImpl2.dll</TestedCodeElem>"
		"<TestedCodeElem>debug/CarImpl3.dll</TestedCodeElem>"
		"</TestElement>";

};

#endif