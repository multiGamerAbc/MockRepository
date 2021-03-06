////////////////////////////////////////////////////////////////////////////////
// LogicImpl2.cpp - Specialization of Logic class                             //
// version 2.48                                                               //
// Language:    C++								                              //
// Application: Spring OOD Project 2                                          //
// Author:      Phil Maalouf, pmaalouf@syr.edu                                //
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "LogicImpl2.h"

////////////////////////////////////////////////////////////////////////////////
// LogicImpl2 Functions - specialize the logic interface class Logic
////////////////////////////////////////////////////////////////////////////////

//----< return implementation identification >----------------------------------
std::string LogicImpl2::getName() {
	return "LogicImpl2";
}
//----< return logical conjunction >--------------------------------------------
bool LogicImpl2::conj(bool p, bool q) {
	return p && q;
}
//----< return logical disjunction >--------------------------------------------
bool LogicImpl2::disj(bool p, bool q) {
	return p || q;
}
//----< return logical imiplies >-----------------------------------------------
bool LogicImpl2::implies(bool p, bool q) {
	return p || !q;
}

//----< Factory Method >--------------------------------------------------------

extern "C" TEST_API IGetTested* getCtor() { return new LogicImpl2(); }


//----< Test Stub >--------------------------------------------------------

#ifdef TEST_LOGICIMPL2

int main() {
	cout << "Test Stub For LogicImpl2" << endl;
	cout << "========================" << endl;
	LogicImpl2 logic;
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