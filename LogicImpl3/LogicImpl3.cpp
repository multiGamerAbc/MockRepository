////////////////////////////////////////////////////////////////////////////////
// LogicImpl3.cpp - Specialization of Logic class                             //
// version 2.48                                                               //
// Language:    C++								                              //
// Application: Spring OOD Project 2                                          //
// Author:      Phil Maalouf, pmaalouf@syr.edu                                //
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "LogicImpl3.h"

////////////////////////////////////////////////////////////////////////////////
// LogicImpl3 Functions - specialize the logic interface class Logic
////////////////////////////////////////////////////////////////////////////////

//----< return implementation identification >----------------------------------

std::string LogicImpl3::getName() {
	return "LogicImpl3";
}

//----< return logical conjunction >--------------------------------------------

bool LogicImpl3::conj(bool p, bool q) {
	return p && q;
}

//----< return logical disjunction >--------------------------------------------

bool LogicImpl3::disj(bool p, bool q) {
	return p || q;
}

//----< return logical imiplies >-----------------------------------------------

bool LogicImpl3::implies(bool p, bool q) {
	return !p || q;
}

//----< Factory Method >--------------------------------------------------------

extern "C" TEST_API IGetTested* getCtor() { return new LogicImpl3(); }


//----< Test Stub >--------------------------------------------------------

#ifdef TEST_LOGICIMPL3

int main() {
	cout << "Test Stub For LogicImpl3" << endl;
	cout << "========================" << endl;
	LogicImpl3 logic;
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