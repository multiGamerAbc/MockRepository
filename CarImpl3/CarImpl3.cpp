////////////////////////////////////////////////////////////////////////////////
// CarImp3.cpp - Specialization of Car class                                  //
// version 2.48                                                               //
// Language:    C++								                              //
// Application: Spring OOD Project 2                                          //
// Author:      Phil Maalouf, pmaalouf@syr.edu                                //
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CarImpl3.h"

////////////////////////////////////////////////////////////////////////////////
// CarImpl3 Functions - specialize the logic interface class Logic
////////////////////////////////////////////////////////////////////////////////

//----< return implementation identification >----------------------------------

std::string CarImpl3::getName() {
	return "CarImpl3";
}

//----< return attribute type >-------------------------------------------------

std::string CarImpl3::getType() {
	return "super fast";
}

//----< return tire tread >-----------------------------------------------------

float CarImpl3::tireTread() {
	return 3.0 / 32;
}

//----< return brake rating >---------------------------------------------------

double CarImpl3::brakeRating() {
	return 100;
}

//----< return max speed >------------------------------------------------------

int CarImpl3::maxSpeed() {
	return 210;
}

//----< Factory Method >--------------------------------------------------------

extern "C" TEST_API IGetTested* getCtor() { return new CarImpl3(); }


//----< Test Stub >-------------------------------------------------------------

#ifdef TEST_CARIMPL3

int main() {
	cout << "Test Stub For CarImpl3" << endl;
	cout << "========================" << endl;

	CarImpl3 car;
	cout << "car.getName()" << car.getName().c_str() << endl;
	cout << "car.tireTread()" << car.tireTread() << endl;
	cout << "car.brakeRating()" << car.brakeRating() << endl;
	cout << "car.maxSpeed()" << car.maxSpeed() << endl;

	cout << endl << "End Test Stub" << endl;
}

#endif