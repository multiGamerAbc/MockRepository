////////////////////////////////////////////////////////////////////////////////
// MathTestDriver.cpp - Tests an implementation of Math                       //
// version 2.48                                                               //
// Language:    C++								                              //
// Application: Spring OOD Project 2                                          //
// Author:      Phil Maalouf, pmaalouf@syr.edu                                //
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "MathTestDriver.h"

////////////////////////////////////////////////////////////////////////////////
// MathTestDriver Functions - specialize the ITest interface
////////////////////////////////////////////////////////////////////////////////

bool MathTestDriver::processTestedCodeElement(IGetTested* testedCodeObj) {
	pMath = (Math*)testedCodeObj;
	return test();
}

bool MathTestDriver::testAdd_1() {
	int returnVal = pMath->add(99, -100);
	bool testResult = returnVal == -1;
	log.newLine();
	log.test("add(99,-100)");
	log.returns(to_string(returnVal));
	log.testResult(to_string(testResult));
	return testResult;
}
bool MathTestDriver::testAdd_2() {

	int returnVal = pMath->add(16,9);
	bool testResult = returnVal == 25;
	log.newLine();
	log.test("add(16,9)");
	log.returns(to_string(returnVal));
	log.testResult(to_string(testResult));
	return testResult;
}
bool MathTestDriver::testSubtract_1() {
	int returnVal = pMath->subtract(99, -100);
	bool testResult = returnVal == 199;
	log.newLine();
	log.test("subtract(99,-100)");
	log.returns(to_string(returnVal));
	log.testResult(to_string(testResult));
	return testResult;
}
bool MathTestDriver::testSubtract_2() {
	int returnVal = pMath->subtract(16,9);
	bool testResult = returnVal == 7;
	log.newLine();
	log.test("subtract(16,9)");
	log.returns(to_string(returnVal));
	log.testResult(to_string(testResult));
	return testResult;
}
bool MathTestDriver::testMultiply_1() {
	int returnVal = pMath->multiply(99, -100);
	bool testResult = returnVal == -9900;
	log.newLine();
	log.test("multiply(99,-100)");
	log.returns(to_string(returnVal));
	log.testResult(to_string(testResult));
	return testResult;
}
bool MathTestDriver::testMultiply_2() {
	int returnVal = pMath->multiply(16,9);
	bool testResult = returnVal == 144;
	log.newLine();
	log.test("multiply(16,9)");
	log.returns(to_string(returnVal));
	log.testResult(to_string(testResult));
	return testResult;
}
bool MathTestDriver::test() {
	log.newLine();
	log.inter("Math");
	log.impl(pMath->getName().c_str());

	results.clear();

	results.push_back(testAdd_1());
	results.push_back(testAdd_2());
	results.push_back(testSubtract_1());
	results.push_back(testSubtract_2());
	results.push_back(testMultiply_1());
	results.push_back(testMultiply_2());

	log.testResultVector(results);

	for (bool testResult : results) {
	 	if (!testResult)
			return false;
	}
	return true;
}

//----< Factory Method >--------------------------------------------------------

extern "C" TEST_API ITest* getCtor() { return new MathTestDriver(); }

//----< Test Stub >-------------------------------------------------------------

#ifdef TEST_MATHTESTDRIVER

int main() {
	MathTestDriver testDriver;
	MathImpl1 math1;
	MathImpl2 math2;
	Math* pMath1 = &math1;
	Math* pMath2 = &math2;
	std::vector<Math*> vecOfpMaths{ pMath1, pMath2 };
	for (Math* pMath : vecOfpMaths)
		cout << pMath->getName() << "    test result:  "
		<< testDriver.processTestedCodeElement(pMath) << endl;
	cout << endl;
}

#endif