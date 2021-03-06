////////////////////////////////////////////////////////////////////////////////
// MathImpl1.cpp - Specialization of Math class                               //
// version 2.48                                                               //
// Language:    C++								                              //
// Application: Spring OOD Project 2                                          //
// Author:      Phil Maalouf, pmaalouf@syr.edu                                //
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "MathImpl1.h"

////////////////////////////////////////////////////////////////////////////////
// MathImpl1 Functions - specialize the math interface class Math
////////////////////////////////////////////////////////////////////////////////

//----< return implementation identification >----------------------------------
std::string MathImpl1::getName() {
	return "MathImpl1";
}
//----< arithmetic add >--------------------------------------------------------
int MathImpl1::add(int a, int b) {
	return a + b;
}
//----< arithmetic subtract >---------------------------------------------------
int MathImpl1::subtract(int a, int b) {
	return a - b;
}
//----< arithmetic multiply >---------------------------------------------------
int MathImpl1::multiply(int a, int b) {
	int result = 0;
	for (int i = 1; i < b; i++) {  // oops
		result += a;
	}
	return result;
}

//----< Factory Method >--------------------------------------------------------

extern "C" TEST_API IGetTested* getCtor() { return new MathImpl1(); }


//----< Test Stub >-------------------------------------------------------------

#ifdef TEST_MATHIMPL1

int main() {
	cout << "Test Stub For MathImpl1" << endl;
	cout << "=======================" << endl;
	MathImpl1 math;
	cout << "math.add(2, 4):         " << math.add(2, 4)       << endl;
	cout << "math.add(2, -4)         " << math.add(2, -4)      << endl;
	cout << "math.subtract(2, 4)     " << math.subtract(2, 4)  << endl;
	cout << "math.subtract(2, -4)    " << math.subtract(2, -4) << endl;
	cout << "math.multiply(2, 4)     " << math.multiply(2, 4)  << endl;
	cout << "math.multiply(2, -4)    " << math.multiply(2, -4) << endl;
	cout << endl << "End Test Stub" << endl;
}

#endif