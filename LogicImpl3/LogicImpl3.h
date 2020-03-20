////////////////////////////////////////////////////////////////////////////////
// LogicImpl3.h - Specialization of Logic class                               //
// version 2.48                                                               //
// Language:    C++								                              //
// Application: Spring OOD Project 2                                          //
// Author:      Phil Maalouf, pmaalouf@syr.edu                                //
////////////////////////////////////////////////////////////////////////////////
#ifndef LOGIC_IMPL_3_H
#define LOGIC_IMPL_3_H
#include "stdafx.h"
#include <iostream>
#include "../TestHarnessUtilities/ITestIGetTested.h"
#include "../Logic/Logic.h"
#include "DllHeader.h"
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// LogicImpl3 - specializes the logic interface class Logic
////////////////////////////////////////////////////////////////////////////////

class TEST_API LogicImpl3 : public Logic {
public:
	std::string getName();
	bool conj(bool p, bool q);
	bool disj(bool p, bool q);
	bool implies(bool p, bool q);
};

#endif