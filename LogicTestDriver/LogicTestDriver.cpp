////////////////////////////////////////////////////////////////////////////////
// LogicTestDriver.cpp - Tests an implementation of Logic                     //
// version 2.48                                                               //
// Language:    C++								                              //
// Application: Spring OOD Project 2                                          //
// Author:      Phil Maalouf, pmaalouf@syr.edu                                //
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "LogicTestDriver.h"

////////////////////////////////////////////////////////////////////////////////
// LogicTestDriver Functions - specialize the ITest interface
////////////////////////////////////////////////////////////////////////////////

	bool LogicTestDriver::processTestedCodeElement(IGetTested* testedCodeObj) {
		pLogic = (Logic*)testedCodeObj;
		return test();
	}

	bool LogicTestDriver::testConj_1() {
		bool returnVal = pLogic->conj(0, 1);
		bool testResult = returnVal == 0;
		log.newLine();
		log.test("conj(0, 1)");
		log.returns(to_string(returnVal));
		log.testResult(to_string(testResult));
		return testResult;
	}

	bool LogicTestDriver::testConj_2() {
		bool returnVal = pLogic->conj(1, 1);
		bool testResult = returnVal == 1;
		log.newLine();
		log.test("conj(1, 1)");
		log.returns(to_string(returnVal));
		log.testResult(to_string(testResult));
		return testResult;
	}

	bool LogicTestDriver::testDisj_1() {
		bool returnVal = pLogic->disj(0, 0);
		bool testResult = returnVal == 0;
		log.newLine();
		log.test("disj(0, 0)");
		log.returns(to_string(returnVal));
		log.testResult(to_string(testResult));
		return testResult;
	}

	bool LogicTestDriver::testDisj_2() {
		bool returnVal = pLogic->disj(0, 1);
		bool testResult = returnVal == 1;
		log.newLine();
		log.test("disj(0, 1)");
		log.returns(to_string(returnVal));
		log.testResult(to_string(testResult));
		return testResult;
	}

	bool LogicTestDriver::testImplies_1() {
		bool returnVal = pLogic->implies(0, 1);
		bool testResult = returnVal == 1;
		log.newLine();
		log.test("implies(0, 1)");
		log.returns(to_string(returnVal));
		log.testResult(to_string(testResult));
		return testResult;
	}

	bool LogicTestDriver::testImplies_2() {
		bool returnVal = pLogic->implies(1, 0);
		bool testResult = returnVal == 0;
		log.newLine();
		log.test("implies(1, 0)");
		log.returns(to_string(returnVal));
		log.testResult(to_string(testResult));
		return testResult;
	}

	bool LogicTestDriver::test() {
		log.newLine();
		log.inter("Logic");
		log.impl(pLogic->getName().c_str());

		results.clear();

		results.push_back(testConj_1());
		results.push_back(testConj_2());
		results.push_back(testDisj_1());
		results.push_back(testDisj_2());
		results.push_back(testImplies_1());
		results.push_back(testImplies_2());

		log.testResultVector(results);

		for (bool testResult : results) {
			if (!testResult)
				return false;
		}
		return true;
	}

//----< Factory Method >--------------------------------------------------------

extern "C" TEST_API ITest* getCtor() { return new LogicTestDriver(); }

//----< Test Stub >-------------------------------------------------------------

#ifdef TEST_LOGICTESTDRIVER
// depends on imports causing unresolved external issue
int main() {
	LogicTestDriver testDriver;
	LogicImpl1 logic1;
	LogicImpl2 logic2;
	LogicImpl3 logic3;
	Logic* pLogic1 = &logic1;
	Logic* pLogic2 = &logic2;
	Logic* pLogic3 = &logic3;
	std::vector<Logic*> vecOfpLogics{ pLogic1, pLogic2, pLogic3 };
	for (Logic* pLogic : vecOfpLogics)
		cout << pLogic->getName() << "    test result:  "
		<< testDriver.processTestedCodeElement(pLogic) << endl;
	cout << endl;
}

#endif
