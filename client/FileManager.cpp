///////////////////////////////////////////////////////////////////////
// FileManager.cpp - Dll spec "server" for Local Test harness        //
// version 2.48                                                      //
// Language:    C++								                     //
// Application: Spring OOD Project 2                                 //
// Author:      Phil Maalouf, pmaalouf@syr.edu                       //
///////////////////////////////////////////////////////////////////////
#include "FileManager.h"

////////////////////////////////////////////////////////////////////////////////
// FileManager - functions
////////////////////////////////////////////////////////////////////////////////

//----< prompt user with file options >-----------------------------------------

void FileManager::prompt() {
	cout << "Options:" << endl;
	cout << "    q:    Quit" << endl;
	cout << "    0:    Demo" << endl;
	cout << "    1:    Math" << endl;
	cout << "    2:    Logic" << endl;
	cout << "    3:    Car" << endl;
	cout << "    4:    Math and Logic" << endl;
	cout << "    5:    Math and Car" << endl;
	cout << "    6:    Logic and Car" << endl;
	cout << "    7:    Math and Logic and Car (demo)" << endl;
}

//----< return XML test request string >----------------------------------------

std::string FileManager::buildRequest(int mode) {
	if (mode == 0) {
		return MathTestRequest + LogicTestRequest + CarTestRequest;
	}
	else if (mode == 1) {
		return MathTestRequest;
	}
	else if (mode == 2) {
		return LogicTestRequest;
	}
	else if (mode == 3) {
		return CarTestRequest;
	}
	else if (mode == 4) {
		return MathTestRequest + LogicTestRequest;
	}
	else if (mode == 5) {
		return MathTestRequest + CarTestRequest;
	}
	else if (mode == 6) {
		return LogicTestRequest + CarTestRequest;
	}
	else if (mode == 7) {
		return FileManager::buildRequest();
	}
	else {
		return "";
	}
}


//----< test stub >-------------------------------------------------------------

#ifdef TEST_FILEMANAGER

int main() {
	cout << "Test stub for FileManager Package" << endl;
	cout << "=================================" << endl << endl;

	FileManager manager;
	cout << "Calling prompt..." << endl << endl;
	manager.prompt();

	cout << endl << "Calling buildRequest in three different modes..." 
		<< endl << endl;

	// These are printed as is, no newline characters are added
	cout << manager.buildRequest(1).c_str() << endl << endl;
	cout << manager.buildRequest(4).c_str() << endl << endl;
	cout << manager.buildRequest().c_str() << endl << endl;

	cout << "End FileManager test stub" << endl;
	cin.get();
}

#endif