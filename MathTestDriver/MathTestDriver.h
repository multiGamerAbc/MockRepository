////////////////////////////////////////////////////////////////////////////////
// MathTestDriver.h - Tests an implementation of Math                         //
// version 2.48                                                               //
// Language:    C++								                              //
// Application: Spring OOD Project 2                                          //
// Author:      Phil Maalouf, pmaalouf@syr.edu                                //
////////////////////////////////////////////////////////////////////////////////
#ifndef MATHTESTDRIVER_H
#define MATHTESTDRIVER_H
#include "stdafx.h"
#include "../Math/Math.h"
#include "../TestHarnessUtilities/ITestIGetTested.h"
#include "../TestHarnessUtilities/MyLogger.h"
#include <iostream>
#include <vector>
#include "DllHeader.h"
//#include "../MathImpl1/MathImpl1.h" // needed for test stub, not Dll
//#include "../MathImpl2/MathImpl2.h" // causes unresolved external issue
									  // and multiply defined DllMain
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// MathTestDriver - specializes the ITest interface
////////////////////////////////////////////////////////////////////////////////

class TEST_API MathTestDriver : public ITest {
public:
	virtual ~MathTestDriver() {	}

	bool processTestedCodeElement(IGetTested* testedCodeObj);

	bool test();

	bool testAdd_1();
	bool testAdd_2();
	bool testSubtract_1();
	bool testSubtract_2();
	bool testMultiply_1();
	bool testMultiply_2();

private:
	Math * pMath;
	MyLogger log;
	std::vector<bool> results;
};

#endif