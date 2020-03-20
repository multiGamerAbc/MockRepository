////////////////////////////////////////////////////////////////////////////////
// Car.h - Declares a car interface to be implemented and tested              //
// version 2.48                                                               //
// Language:    C++								                              //
// Application: Spring OOD Project 2                                          //
// Author:      Phil Maalouf, pmaalouf@syr.edu                                //
////////////////////////////////////////////////////////////////////////////////
#ifndef CAR_H
#define CAR_H
#include <iostream>
#include "../TestHarnessUtilities/ITestIGetTested.h"

class Car : public IGetTested {
public:
	virtual std::string getName() = 0;
	virtual std::string getType() = 0;
	virtual float tireTread() = 0;
	virtual double brakeRating() = 0;
	virtual int maxSpeed() = 0;

private:


}; // OK




#endif