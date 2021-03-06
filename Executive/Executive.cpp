#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include "imagehlp.h"
#include <vector>
#include "Executive.h"
#include "TestHarness.h"

using namespace std;


int main()
{
	cout << "creating harness..\n";
	TestHarness harness;

	harness.processAllElements("singleXmlStringLiteral");

	std::cin.get();
}



////////////////////////////////////////////////////////////////////////////////
// ---
////////////////////////////////////////////////////////////////////////////////









///////////////////////////////////////////////////////////////
// Executive.cpp - Demonstrate Programming to Interface
//
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2005
///////////////////////////////////////////////////////////////
// Build Process:
///////////////////////////////////////////////////////////////
// This workspace contains three projects:
//   testv1 - Defines CTest version 1.0 built as library dll
//   testv2 - Defines CTest version 2.0 built as library dll
//   Executive - implicitly loads one of these test libraries
//
// Each testv? project builds its dll in the debug directory
// of the Executive, where the Executive can load it.
// Rebuild testv1, then Executive
// Run the Executive without rebuilding - see version 1.0 message
// Rebuild testv2
// Run the Executive without rebuilding - see version 2.0 message
//
// Note:
//   For this demo to work correctly you must follow the steps
//   above and:
//     CLICK NO ON THE DIALOG THAT TELLS YOU THE PROJECT
//     CONFIGURATION(S) ARE OUT OF DATE AND DO YOU WANT
//     TO REBUILD.
//   The demo is trying to show you that the Executive does not
//   have to be rebuilt.  Just copy the new test dll into the
//   Executive's bin\debug directory, which is done automatically
//   because I set up the project properties to deposit the
//   new dll into the Executives bin\debug.
///////////////////////////////////////////////////////////////
