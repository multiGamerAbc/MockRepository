////////////////////////////////////////////////////////////////////////////////
// LogicImpl1.cpp - Specialization of Logic class                             //
// version 2.48                                                               //
// Language:    C++								                              //
// Application: Spring OOD Project 2                                          //
// Author:      Phil Maalouf, pmaalouf@syr.edu                                //
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "LogicImpl1.h"

////////////////////////////////////////////////////////////////////////////////
// LogicImpl1 Functions - specialize the logic interface class Logic
////////////////////////////////////////////////////////////////////////////////

//----< return implementation identification >----------------------------------
std::string LogicImpl1::getName() {
	return "LogicImpl1";
}
//----< return logical conjunction >--------------------------------------------
bool LogicImpl1::conj(bool p, bool q) {
	return p && q;
}
//----< return logical disjunction >--------------------------------------------
bool LogicImpl1::disj(bool p, bool q) {
	return p + q + 1;
}
//----< return logical imiplies >-----------------------------------------------
bool LogicImpl1::implies(bool p, bool q) {
	cout << "### LogicIimpl1" << endl;
	return !p || q;
}

//----< Factory Method >--------------------------------------------------------

extern "C" TEST_API IGetTested* getCtor() { return new LogicImpl1(); }


//----< Test Stub >-------------------------------------------------------------

#ifdef TEST_LOGICIMPL1

int main() {
	cout << "Test Stub For LogicImpl1" << endl;
	cout << "========================" << endl;
	LogicImpl1 logic;
	cout << "logic.conj(true,true)" << logic.conj(true, true) << endl;
	cout << "logic.conj(true,false)" << logic.conj(true, false) << endl;
	cout << "logic.conj(false,true)" << logic.conj(false, true) << endl;
	cout << "logic.conj(false,false)" << logic.conj(false, false) << endl;

	cout << "logic.disj(true,true)" << logic.disj(true, true) << endl;
	cout << "logic.disj(true,false)" << logic.disj(true, false) << endl;
	cout << "logic.disj(false,true)" << logic.disj(false, true) << endl;
	cout << "logic.disj(false,false)" << logic.disj(false, false) << endl;

	cout << "logic.implies(true,true)" << logic.implies(true, true) << endl;
	cout << "logic.implies(true,false)" << logic.implies(true, false) << endl;
	cout << "logic.implies(false,true)" << logic.implies(false, true) << endl;
	cout << "logic.implies(false,false)" << logic.implies(false, false) << endl;

	cout << endl << "End Test Stub" << endl;
}

#endif