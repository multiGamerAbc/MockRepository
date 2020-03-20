////////////////////////////////////////////////////////////////////////////////
// Math.h - Declares a math interface to be implemented and tested           //
// version 2.48                                                               //
// Language:    C++								                              //
// Application: Spring OOD Project 2                                          //
// Author:      Phil Maalouf, pmaalouf@syr.edu                                //
////////////////////////////////////////////////////////////////////////////////
#ifndef MATH_H
#define MATH_H
#include <iostream>
#include "../TestHarnessUtilities/ITestIGetTested.h"

////////////////////////////////////////////////////////////////////////////////
// Math - abstract class serving as math interface
////////////////////////////////////////////////////////////////////////////////

class Math : public IGetTested {
public:
	virtual std::string getName() {
		return "name is Math";
	}
	virtual int add(int, int) = 0;
	virtual int subtract(int, int) = 0;
	virtual int multiply(int, int) = 0;
};

#endif