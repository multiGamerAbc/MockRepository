////////////////////////////////////////////////////////////////////////////////
// MyLogger.h - Utility for logging test results                                //
// version 2.48                                                               //
// Language:    C++								                              //
// Application: Spring OOD Project 2                                          //
// Author:      Phil Maalouf, pmaalouf@syr.edu                                //
////////////////////////////////////////////////////////////////////////////////
#ifndef MYLOGGER_H
#define MYLOGGER_H
#include <iostream>
#include <chrono>
#include <locale>
#include <sstream>
#include <vector>
#include <fstream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// MyLogger -- handles all logging activities for Local Test Harness project
////////////////////////////////////////////////////////////////////////////////
// Note:
// I had trouble trying to define the functions of the logger class externally
// to the class body.  Therefore I have included a class declaration as comments
// so that it will be easy to see the functions which are defined for the class.

//class MyLogger {
//public:
//	ostringstream time();
//	void newLine(int count = 1);
//	void wrapEquals(const std::string str, int mode = 1);
//	void wrapDash(const std::string str, int mode = 1);
//	void wrapSlashRight(const std::string str, int mode = 1);
//	std::string multStr(std::string item, int factor);
//	void wrapDashRightWithSlash(const std::string str, int mode = 1);
//	void inter(const std::string str);
//	void impl(const std::string str);
//	void test(const std::string str);
//	void returns(const std::string str);
//	void testResult(const std::string str);
//	void testResultVector(std::vector<bool> subTestResults);
//};

class MyLogger {
public:

//-------< get current time >---------------------------------------------------

	ostringstream time(){
		ostringstream result;
		auto now = chrono::system_clock::now();
		std::time_t timeString = chrono::system_clock::to_time_t(now);
		tm* nowTM = std::localtime(&timeString);

		locale locC;
		const time_put<char>& tpC = use_facet<time_put<char>>(locC);

		string format = "%c\n";
		tpC.put(result, result, ' ', nowTM,
			format.c_str(), format.c_str() + format.size());  // From Text
	
		result.seekp(-56, std::ios_base::end);
		return result;
	}


//-------< log a newLine >------------------------------------------------------

	void newLine(int count = 1) {
		for (int i = 0; i < count; i++)
			cout << "\n";
	}

//-------< log string wrapped in '=' char >-------------------------------------

	void wrapEquals(const std::string str, int mode = 1) {
		ofstream myfile;
		myfile.open("../example32.txt");
		myfile << "Writing this to a file.\n";
		myfile.close();
		if (str.size() == 0) {
			cout << multStr("=", 80).c_str() << endl;
		}
		else
			if (str.size() > 80) {
				cout << str.c_str() << endl;
			}
			else {
				std::string buf = multStr("=", (80 - str.size() - 2) / 2);
				cout << multStr("=", str.size() % 2).c_str() << buf.c_str() 
					<< " " << str.c_str() << " " << buf.c_str() << endl;
			}
	}

//-------< log string wrapped in '-' char >-------------------------------------

	void wrapDash(const std::string str, int mode = 1) {
		if (str.size() == 0) {
			cout << multStr("-", 80).c_str() << endl;
		}
		else
			if (str.size() > 80) {
				cout << str.c_str() << endl;
			}
			else {
				std::string buf = multStr("-", (80 - str.size() - 2) / 2);
				cout << multStr("-", str.size() % 2).c_str() << buf.c_str() 
					<< " " << str.c_str() << " " << buf.c_str() << endl;
			}
	}

//-------< log string wrapped in '/' char positioned right >--------------------

	void wrapSlashRight(const std::string str, int mode = 1) {
		if (str.size() > 80) {
			cout << str.c_str() << endl;
		}
		else {
			std::string leftBuf = multStr("/", (80 - 3 - str.size() - 2));
			std::string rightBuf = multStr("/", 3);
			cout << leftBuf.c_str() << " " << str.c_str() 
				<< " " << rightBuf.c_str() << endl;
		}
	}

//-------< multiply string >----------------------------------------------------

	std::string multStr(std::string item, int factor) {
		std::string result = "";
		for (int i = 0; i < factor; i++) {
			result += item;
		}
		return result;
	}

//-------< log string wrapped in '-' char positioned right with ending "//" >---

	void wrapDashRightWithSlash(const std::string str, int mode = 1) {
		if (str.size() > 74) {
			cout << str.c_str() << endl;
		}
		else {
			std::string leftBuf = multStr("-", (80 - 3 - str.size() - 3));
			cout << leftBuf.c_str() << "// " << str.c_str() << " //" << endl;
		}
	}

//-------< log interface identification >---------------------------------------

	void inter(const std::string str) {
		cout << "Interface:        " << str << endl;
	}

//-------< log implementation identification >----------------------------------
    void impl(const std::string str) {
		cout << "Implementation:   " << str << endl;
	}
//-------< log test identification >--------------------------------------------
	void test(const std::string str) {
		cout << "Test:             " << str << endl;
	}
//-------< log returned value >-------------------------------------------------
	void returns(const std::string str) {
		cout << "Returns:          " << str << endl;
	}
//-------< log subtest result >-------------------------------------------------
	void testResult(const std::string str) {
		cout << "Test Result:      " << str << endl;
	}
//-------< log subtest result vector >------------------------------------------
	void testResultVector(std::vector<bool> subTestResults)	{
		cout << endl;
		cout << "Subtest Results Vector:   ";
		for (bool result : subTestResults)
			cout << "    " << result;
		cout << endl;
	}

};  // End class MyLogger


//void MyLogger::testResultVector(std::vector<bool> subTestResults) {
//	cout << endl;
//	cout << "Subtest Results Vector:   ";
//	for (bool result : subTestResults)
//		cout << "    " << result;
//	cout << endl;
//}



#endif

//-------< MyLogger Test Stub >---------------------------------------------------

#ifdef TEST_LOGGER

int main() {
	MyLogger log;
	log.newLine();
	log.wrapEquals("");
	log.wrapEquals("Test Stub For MyLogger Class");
	log.wrapEquals("");
	log.newLine();
	log.wrapEquals("wrapEquals");
	log.wrapDash("wrapDash");
	log.wrapSlashRight("wrapSlashRight");
	log.wrapDashRightWithSlash("wrapDashRightWithSlash");
	log.wrapDash(log.multStr("multStr(literal,3)_in_wrapDash", 3));
	log.inter("calling inter");
	log.impl("calling impl");
	log.test("calling test");
	log.returns("calling returns");
	log.testResult("calling testResult");
	std::vector<bool> demonstrationVector = { 1,0,1,0,1,1,1,1,0,0,0,0,1,0 };
	log.testResultVector(demonstrationVector);
	log.wrapSlashRight(log.time().str().substr(0, 17));
	log.wrapDash("");
	log.wrapDash("End MyLogger Test Stub");
	log.wrapDash("");
}

#endif
