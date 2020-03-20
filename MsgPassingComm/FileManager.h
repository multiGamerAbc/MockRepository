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
#include <vector>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// FileManager - declaraction
////////////////////////////////////////////////////////////////////////////////

class RequestSet {
public:
	RequestSet() {
		requestVector.push_back(req1);
		requestVector.push_back(req2);
		requestVector.push_back(req3);
		requestVector.push_back(req4);
		requestVector.push_back(req5);
		requestVector.push_back(req6);
		requestVector.push_back(req7);
		requestVector.push_back(req8);
		requestVector.push_back(req9);
		requestVector.push_back(req10);
	}
	//----< index operator for indexing requestVector >-----------------------
	std::string operator[](int i) {
		return requestVector[i];
	}
	//----< returns requestVector >-------------------------------------------
	std::vector<std::string> getVector() {
		return requestVector;
	}
private:
	std::vector<std::string> requestVector;

	std::string req1 = "<TestElement>"
		"<TestDriverElem>MathTestDriver</TestDriverElem>"
		"<TestedCodeElem>MathImpl1</TestedCodeElem>"
		"</TestElement>";
	std::string req2 = "<TestElement>"
		"<TestDriverElem>MathTestDriver</TestDriverElem>"
		"<TestedCodeElem>MathImpl2</TestedCodeElem>"
		"</TestElement>";
	std::string req3 = "<TestElement>"
		"<TestDriverElem>LogicTestDriver</TestDriverElem>"
		"<TestedCodeElem>LogicImpl1</TestedCodeElem>"
		"</TestElement>";
	std::string req4 = "<TestElement>"
		"<TestDriverElem>LogicTestDriver</TestDriverElem>"
		"<TestedCodeElem>LogicImpl2</TestedCodeElem>"
		"</TestElement>";
	std::string req5 = "<TestElement>"
		"<TestDriverElem>LogicTestDriver</TestDriverElem>"
		"<TestedCodeElem>LogicImpl3</TestedCodeElem>"
		"</TestElement>";
	std::string req6 = "<TestElement>"
		"<TestDriverElem>CarTestDriver</TestDriverElem>"
		"<TestedCodeElem>CarImpl1</TestedCodeElem>"
		"</TestElement>";
	std::string req7 = "<TestElement>"
		"<TestDriverElem>CarTestDriver</TestDriverElem>"
		"<TestedCodeElem>CarImpl2</TestedCodeElem>"
		"</TestElement>";
	std::string req8 = "<TestElement>"
		"<TestDriverElem>CarTestDriver</TestDriverElem>"
		"<TestedCodeElem>CarImpl3</TestedCodeElem>"
		"</TestElement>";
	std::string req9 = "<TestElement>"
		"<TestDriverElem>MathTestDriver</TestDriverElem>"
		"<TestedCodeElem>MathImpl1</TestedCodeElem>"
		"</TestElement>"
		"<TestElement>"
		"<TestDriverElem>LogicTestDriver</TestDriverElem>"
		"<TestedCodeElem>LogicImpl1</TestedCodeElem>"
		"</TestElement>";
	std::string req10 = "<TestElement>"
		"<TestDriverElem>MathTestDriver</TestDriverElem>"
		"<TestedCodeElem>MathImpl1</TestedCodeElem>"
		"<TestedCodeElem>MathImpl2</TestedCodeElem>"
		"</TestElement>"
		"<TestElement>"
		"<TestDriverElem>LogicTestDriver</TestDriverElem>"
		"<TestedCodeElem>LogicImpl1</TestedCodeElem>"
		"<TestedCodeElem>LogicImpl2</TestedCodeElem>"
		"<TestedCodeElem>LogicImpl3</TestedCodeElem>"
		"</TestElement>"
		"<TestElement>"
		"<TestDriverElem>CarTestDriver</TestDriverElem>"
		"<TestedCodeElem>CarImpl1</TestedCodeElem>"
		"<TestedCodeElem>CarImpl2</TestedCodeElem>"
		"<TestedCodeElem>CarImpl3</TestedCodeElem>"
		"</TestElement>";
};

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