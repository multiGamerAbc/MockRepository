////////////////////////////////////////////////////////////////////////////////
// CarTestDriver.cpp - Tests an implementation of Car                         //
// version 2.48                                                               //
// Language:    C++								                              //
// Application: Spring OOD Project 2                                          //
// Author:      Phil Maalouf, pmaalouf@syr.edu                                //
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CarTestDriver.h"

////////////////////////////////////////////////////////////////////////////////
// CarTestDriver Functions - specialize the ITest interface
////////////////////////////////////////////////////////////////////////////////

	bool CarTestDriver::processTestedCodeElement(IGetTested* testedCodeObj) {
		pCar = (Car*)testedCodeObj;
		return test();
	}

	bool CarTestDriver::testTires() {
		double returnVal = pCar->tireTread();
		bool testResult = returnVal >= 2.0 / 32;
		log.newLine();
		log.test("tireTread()");
		log.returns(to_string(returnVal));
		log.testResult(to_string(testResult));
		return testResult;
	}

	bool CarTestDriver::testBrakes() {
		double returnVal = pCar->brakeRating();
		bool testResult = returnVal >= 75;
		log.newLine();
		log.test("brakeRating()");
		log.returns(to_string(returnVal));
		log.testResult(to_string(testResult));
		return testResult;
	}

	bool CarTestDriver::testSpeed() {
		int returnVal = pCar->maxSpeed();
		bool testResult = returnVal >= 75;
		log.newLine();
		log.test("maxSpeed()");
		log.returns(to_string(returnVal));
		log.testResult(to_string(testResult));
		return testResult;
	}


	bool CarTestDriver::test() {
		log.newLine();
		log.inter("Car");
		log.impl(pCar->getName().c_str());

		results.clear();
		results.push_back(testTires());
		results.push_back(testBrakes());
		results.push_back(testSpeed());

		log.testResultVector(results);

		for (bool testResult : results) {
			if (!testResult)
				return false;
		}
		return true;
	}

//----< Factory Method >--------------------------------------------------------

extern "C" TEST_API ITest* getCtor() { return new CarTestDriver(); }


//----< Test Stub >-------------------------------------------------------------

#ifdef TEST_CARTESTDRIVER
// depends on imports causing unresolved external issues
int main() {
	CarTestDriver testDriver;
	CarImpl1 car1;
	CarImpl2 car2;
	CarImpl3 car3;
	Car* pCar1 = &car1;
	Car* pCar2 = &car2;
	Car* pCar3 = &car3;
	std::vector<Car*> vecOfpCars{ pCar1, pCar2, pCar3 };
	for (Car* pCar : vecOfpCars)
		cout << pCar->getName() << "    test result:  "
		<< testDriver.processTestedCodeElement(pCar) << endl;
	cout << endl;
}

#endif