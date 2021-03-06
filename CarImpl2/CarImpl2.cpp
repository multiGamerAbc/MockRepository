////////////////////////////////////////////////////////////////////////////////
// CarImpl2.cpp - Specialization of Car class                                 //
// version 2.48                                                               //
// Language:    C++								                              //
// Application: Spring OOD Project 2                                          //
// Author:      Phil Maalouf, pmaalouf@syr.edu                                //
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CarImpl2.h"

////////////////////////////////////////////////////////////////////////////////
// CarImpl2 Functions - specialize the logic interface class Logic
////////////////////////////////////////////////////////////////////////////////
//----< return implementation identification >----------------------------------

std::string CarImpl2::getName() {
	return "CarImpl2";
}

//----< return attribute type >-------------------------------------------------

std::string CarImpl2::getType() {
	return "very fast";
}

//----< return tire tread >-----------------------------------------------------

float CarImpl2::tireTread() {
	return 3.0 / 32;
}

//----< return brake rating >---------------------------------------------------

double CarImpl2::brakeRating() {
	return 60;
}

//----< return max speed >------------------------------------------------------

int CarImpl2::maxSpeed() {
	return 140;
}

//----< Factory Method >--------------------------------------------------------

extern "C" TEST_API IGetTested* getCtor() { return new CarImpl2(); }

//----< Test Stub >-------------------------------------------------------------

#ifdef TEST_CARIMPL2

int main() {
	cout << "Test Stub For CarImpl2" << endl;
	cout << "========================" << endl;

	CarImpl2 car;
	cout << "car.getName()" << car.getName().c_str() << endl;
	cout << "car.tireTread()" << car.tireTread() << endl;
	cout << "car.brakeRating()" << car.brakeRating() << endl;
	cout << "car.maxSpeed()" << car.maxSpeed() << endl;

	cout << endl << "End Test Stub" << endl;
}

#endif