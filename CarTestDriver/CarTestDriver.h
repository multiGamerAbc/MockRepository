////////////////////////////////////////////////////////////////////////////////
// CarTestDriver.h - Tests an implementation of Car                           //
// version 2.48                                                               //
// Language:    C++								                              //
// Application: Spring OOD Project 2                                          //
// Author:      Phil Maalouf, pmaalouf@syr.edu                                //
////////////////////////////////////////////////////////////////////////////////
#ifndef CARTESTDRIVER_H
#define CARTESTDRIVER_H
#include "stdafx.h"
#include "../Car/Car.h"
#include "../TestHarnessUtilities/ITestIGetTested.h"
#include "../TestHarnessUtilities/MyLogger.h"
#include <iostream>
#include <vector>
#include "DllHeader.h"
//#include "../CarImpl1/CarImpl1.h"  // used for test stub, not need for Dll
//#include "../CarImpl2/CarImpl2.h"  // caused unresolved external issues
//#include "../CarImpl3/CarImpl3.h"
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// CarTestDriver - specializes the ITest interface
////////////////////////////////////////////////////////////////////////////////

class TEST_API CarTestDriver : public ITest
{
public:
	virtual ~CarTestDriver() {	}

	bool processTestedCodeElement(IGetTested* testedCodeObj);
	
	bool test();

	bool testTires();
	bool testBrakes();
	bool testSpeed();

private:
	Car * pCar;
	MyLogger log;
	std::vector<bool> results;
};

#endif