////////////////////////////////////////////////////////////////////////////////
// CarImpl1.h - Specialization of Car class                                   //
// version 2.48                                                               //
// Language:    C++								                              //
// Application: Spring OOD Project 2                                          //
// Author:      Phil Maalouf, pmaalouf@syr.edu                                //
////////////////////////////////////////////////////////////////////////////////
#ifndef CAR_IMPL_1_H
#define CAR_IMPL_1_H
#include "stdafx.h"
#include <iostream>
#include "../TestHarnessUtilities/ITestIGetTested.h"
#include "../Car/Car.h"
#include "DllHeader.h"
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// CarImpl1 - specializes the car interface class Car
////////////////////////////////////////////////////////////////////////////////

class TEST_API CarImpl1 : public Car {
public:
	std::string getName();
	std::string getType();
	float tireTread();
	double brakeRating();
	int maxSpeed();
};

#endif