////////////////////////////////////////////////////////////////////////////////
// MathImpl1.h - Specialization of Math class                                 //
// version 2.48                                                               //
// Language:    C++								                              //
// Application: Spring OOD Project 2                                          //
// Author:      Phil Maalouf, pmaalouf@syr.edu                                //
////////////////////////////////////////////////////////////////////////////////
#ifndef MATH_IMPL_1_H
#define MATH_IMPL_1_H
#include "stdafx.h"
#include <iostream>
#include "../TestHarnessUtilities/ITestIGetTested.h"
#include "../Math/Math.h"
#include "DllHeader.h"
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// MathImpl1 - specializes the math interface class Math
////////////////////////////////////////////////////////////////////////////////

class TEST_API MathImpl1 : public Math {
public:
	std::string getName();
	int add(int a, int b);
	int subtract(int a, int b);
	int multiply(int a, int b);
};

#endif