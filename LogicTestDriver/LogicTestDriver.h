////////////////////////////////////////////////////////////////////////////////
// LogicTestDriver.h - Tests an implementation of Logic                       //
// version 2.48                                                               //
// Language:    C++								                              //
// Application: Spring OOD Project 2                                          //
// Author:      Phil Maalouf, pmaalouf@syr.edu                                //
////////////////////////////////////////////////////////////////////////////////
#ifndef LOGICTESTDRIVER_H
#define LOGICTESTDRIVER_H
#include "stdafx.h"
#include "../Logic/Logic.h"
#include "../TestHarnessUtilities/ITestIGetTested.h"
#include "../TestHarnessUtilities/MyLogger.h"
#include <iostream>
#include <vector>
#include "DllHeader.h"
//#include "../LogicImpl1/LogicImpl1.h"  // needed for test stub, not Dll
//#include "../LogicImpl2/LogicImpl2.h"  // causes unresolved external issue
//#include "../LogicImpl3/LogicImpl3.h"
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// LogicTestDriver - specializes the ITest interface
////////////////////////////////////////////////////////////////////////////////


class TEST_API LogicTestDriver : public ITest
{
public:
	virtual ~LogicTestDriver() {	}

	bool processTestedCodeElement(IGetTested* testedCodeObj);

	bool test();

	bool testConj_1();
	bool testConj_2();
	bool testDisj_1();
	bool testDisj_2();
	bool testImplies_1();
	bool testImplies_2();

private:
	Logic * pLogic;
	MyLogger log;
	std::vector<bool> results;

};


#endif