////////////////////////////////////////////////////////////////////////////////
// MathImpl2.cpp - Specialization of Math class                               //
// version 2.48                                                               //
// Language:    C++								                              //
// Application: Spring OOD Project 2                                          //
// Author:      Phil Maalouf, pmaalouf@syr.edu                                //
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "MathImpl2.h"

////////////////////////////////////////////////////////////////////////////////
// MathImpl2 - specializes the math interface class Math
////////////////////////////////////////////////////////////////////////////////


class TEST_API MathImpl2 : public Math {
public:
//----< return implementation identification >----------------------------------
	std::string getName() {
		return "MathImpl2";
	}
//----< arithmetic add >--------------------------------------------------------

	int add(int a, int b) {
		return a + b;
	}
//----< arithmetic subtract >---------------------------------------------------

	int subtract(int a, int b) {
		return a - b;
	}
//----< arithmetic multiply >---------------------------------------------------

	int multiply(int a, int b) {
		return a * b;
	}
};

//----< Factory Method >--------------------------------------------------------

extern "C" TEST_API IGetTested* getCtor() { return new MathImpl2(); }


//----< Test Stub >-------------------------------------------------------------

#ifdef TEST_MATHIMPL2

int main() {
	cout << "Test Stub For MathImpl1" << endl;
	cout << "=======================" << endl;
	MathImpl1 math;
	cout << "math.add(2, 4):         " << math.add(2, 4) << endl;
	cout << "math.add(2, -4)         " << math.add(2, -4) << endl;
	cout << "math.subtract(2, 4)     " << math.subtract(2, 4) << endl;
	cout << "math.subtract(2, -4)    " << math.subtract(2, -4) << endl;
	cout << "math.multiply(2, 4)     " << math.multiply(2, 4) << endl;
	cout << "math.multiply(2, -4)    " << math.multiply(2, -4) << endl;
	cout << endl << "End Test Stub" << endl;
}

#endif