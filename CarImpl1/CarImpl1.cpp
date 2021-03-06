////////////////////////////////////////////////////////////////////////////////
// CarImpl1.cpp - Specialization of Car class                                 //
// version 2.48                                                               //
// Language:    C++								                              //
// Application: Spring OOD Project 2                                          //
// Author:      Phil Maalouf, pmaalouf@syr.edu                                //
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CarImpl1.h"

////////////////////////////////////////////////////////////////////////////////
// CarImpl1 Functions - specialize the logic interface class Logic
////////////////////////////////////////////////////////////////////////////////

//----< return implementation identification >----------------------------------

std::string CarImpl1::getName() {
	return "CarImpl1";
}

//----< return attribute type >-------------------------------------------------

std::string CarImpl1::getType() {
	return "kinda fast";
}

//----< return tire tread >-----------------------------------------------------

float CarImpl1::tireTread() {
	return 1.0 / 32;
}

//----< return brake rating >---------------------------------------------------

double CarImpl1::brakeRating() {
	return 95;
}

//----< return max speed >------------------------------------------------------

int CarImpl1::maxSpeed() {
	return 70;
}

//----< Factory Method >--------------------------------------------------------

extern "C" TEST_API IGetTested* getCtor() { return new CarImpl1(); }


//----< Test Stub >-------------------------------------------------------------

#ifdef TEST_CARIMPL1

int main() {
	cout << "Test Stub For CarImpl1" << endl;
	cout << "========================" << endl;

	CarImpl1 car;
	cout << "car.getName()" << car.getName().c_str() << endl;
	cout << "car.tireTread()" << car.tireTread() << endl;
	cout << "car.brakeRating()" << car.brakeRating() << endl;
	cout << "car.maxSpeed()" << car.maxSpeed() << endl;

	cout << endl << "End Test Stub" << endl;
}

#endif