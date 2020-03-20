////////////////////////////////////////////////////////////////////////////////
// ITestIGetTested.h - Defines interfaces for tested code and test drivers    //
// version 2.48                                                               //
// Language:    C++								                              //
// Application: Spring OOD Project 2                                          //
// Author:      Phil Maalouf, pmaalouf@syr.edu                                //
////////////////////////////////////////////////////////////////////////////////
#ifndef ITESTIGETTESTEDPAIR_H
#define ITESTIGETTESTEDPAIR_H

////////////////////////////////////////////////////////////////////////////////
// IGetTested  --  an interface handle for tested code elements
////////////////////////////////////////////////////////////////////////////////

struct IGetTested {
public:
	virtual ~IGetTested() {}
};

////////////////////////////////////////////////////////////////////////////////
// ITest  --  an interface for test driver elements
////////////////////////////////////////////////////////////////////////////////

struct ITest {
public:
	virtual ~ITest() {}
	virtual bool processTestedCodeElement(IGetTested* obj) = 0;
	virtual bool test() = 0;
};

#endif