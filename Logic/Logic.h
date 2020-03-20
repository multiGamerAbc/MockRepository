////////////////////////////////////////////////////////////////////////////////
// Logic.h - Declares a logic interface to be implemented and tested          //
// version 2.48                                                               //
// Language:    C++								                              //
// Application: Spring OOD Project 2                                          //
// Author:      Phil Maalouf, pmaalouf@syr.edu                                //
////////////////////////////////////////////////////////////////////////////////
#ifndef LOGIC_H
#define LOGIC_H
#include <iostream>
#include "../TestHarnessUtilities/ITestIGetTested.h"

class Logic : public IGetTested {
public:
	virtual std::string getName() {
		return "name is Logic";
	}
	virtual bool conj(bool, bool) = 0;
	virtual bool disj(bool, bool) = 0;
	virtual bool implies(bool, bool) = 0;
};




#endif