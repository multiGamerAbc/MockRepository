///////////////////////////////////////////////////////////////////////////
// Comm.h - message-passing communication facility                       //
// ver 2.0 (Modified for CSE687 OOD Project 4, Mock Repository)          //
// Author: Phil Maalouf, pmaalouf@syr.edu                                //
// Source: Jim Fawcett, CSE687-OnLine Object Oriented Design, Fall 2017  //
///////////////////////////////////////////////////////////////////////////
//Cite: https://stackoverflow.com/questions/5167625/splitting-a-c-stdstring-using-tokens-e-g

#include <algorithm>
#include "CommServer.h"
#include <conio.h>
#include <exception>
#include "FileManager.h"
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include "TestHarness.h"
#include <vector>
#include "XmlReader.h"

#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "../FileSystemDemo/FileSystem.h"
#include "../Logger/Logger.h"
#include "../TestHarnessUtilities/MyLogger.h"
#include "../TestHarnessUtilities/TestElement.h"
#include "../Utilities/Utilities.h"

/////////////////////////////////////////////////////////
// helper FileSystemSearch

class FileSystemSearch
{
public:
	FileSystemSearch();
	~FileSystemSearch();
	std::string firstFile(
		const std::string& path = ".", const std::string& pattern = "*.*");
	std::string nextFile();
	std::string firstDirectory(
		const std::string& path = ".", const std::string& pattern = "*.*");
	std::string nextDirectory();
	void close();
private:
	HANDLE hFindFile;
	WIN32_FIND_DATAA FindFileData;
	WIN32_FIND_DATAA* pFindFileData;
};

//----< variables (set 1) >-----------------------------------------------------

using namespace std;
using namespace MsgPassingCommunication;
using namespace Sockets;
using SUtils = Utilities::StringHelper;
std::string saveFilePath = "../SaveFiles";
std::string sendFilePath = "../SendFiles";
const size_t BlockSize = 1024;
Socket::byte rwBuffer[BlockSize];

//----< variables (set 2) >-----------------------------------------------------

std::string saveFilePathTemp = "";
std::string sendFilePathTemp = "";
int testRequestID = 1;
SocketSystem ss;
int contrivedExtractorDelayTime = 20;
std::mutex ioMutex;
BlockingQueue<Message> testRequestBQ;
BlockingQueue<Message> readyMessageBQ;

int serverPortNumForServerUserCommunication = 7000;
int serverPortNumForServerCtpCommunication_1 = 7001;
int serverPortNumForServerCtpCommunication_2 = 7002;
int serverPortNumForServerCtpCommunication_3 = 7003;

int aUserPortNumForServerUserCommunication = 5000;
int ctpPortNumForServerCtpCommunication_1 = 3001;
int ctpPortNumForServerCtpCommunication_2 = 3002;
int ctpPortNumForServerCtpCommunication_3 = 3003;

EndPoint serverEPforServerUserCommunication(
	"localhost", serverPortNumForServerUserCommunication);
EndPoint serverEPforServerCtpCommunication_1(
	"localhost", serverPortNumForServerCtpCommunication_1);
EndPoint serverEPforServerCtpCommunication_2(
	"localhost", serverPortNumForServerCtpCommunication_2);
EndPoint serverEPforServerCtpCommunication_3(
	"localhost", serverPortNumForServerCtpCommunication_3);

EndPoint aUserEPforServerUserCommunication(
	"localhost", aUserPortNumForServerUserCommunication);
EndPoint ctpEPforServerCtpCommunication_1(
	"localhost", ctpPortNumForServerCtpCommunication_1);
EndPoint ctpEPforServerCtpCommunication_2(
	"localhost", ctpPortNumForServerCtpCommunication_2);
EndPoint ctpEPforServerCtpCommunication_3(
	"localhost", ctpPortNumForServerCtpCommunication_3);

Comm serverUserComm(serverEPforServerUserCommunication, "serverUserComm");
Comm serverCtpComm_1(serverEPforServerCtpCommunication_1, "serverCtpComm");
Comm serverCtpComm_2(serverEPforServerCtpCommunication_2, "serverCtpComm");
Comm serverCtpComm_3(serverEPforServerCtpCommunication_3, "serverCtpComm");

//----< function declarations >-------------------------------------------------

int spawn(int id);
void loc(const std::string pos);
void communicationTest(int num);
void serverUserCommExtractor();
void serverCtpCommExtractor_1();
void serverCtpCommExtractor_2();
void serverCtpCommExtractor_3();
void matchTestRequestsToReadyCTPs();

//----< search for a directory >------------------------------------------------

std::string searchForDirectory(std::string nameString, std::string searchPath) {
	FileSystemSearch fss;
	std::string dirName = fss.firstDirectory(searchPath);
	if (dirName.size() > 0) {
		if (dirName == nameString)
			return searchPath + "/" + nameString;
		if (dirName != "." && dirName != "..") {
			std::string retVal = searchForDirectory(
				nameString, searchPath + "/" + dirName);
			if (retVal.size() > 0)
				return retVal;
		}
	}
	while (true)
	{
		dirName = fss.nextDirectory();
		if (dirName.size() > 0) {
			if (dirName == nameString)
				return searchPath + "/" + nameString;
			if (dirName != "." && dirName != "..") {
				std::string retVal = searchForDirectory(
					nameString, searchPath + "/" + dirName);
				if (retVal.size() > 0)
					return retVal;
			}
		}
		else
			break;
	}
	return "";
}

//----< search for a file >-----------------------------------------------------

std::string searchForFile(std::string nameString, std::string searchPath) {
	FileSystemSearch fss;
	std::string fileName = fss.firstFile(searchPath);
	if (fileName.size() > 0) {
		if (fileName == nameString) {
			return searchPath + "/" + fileName;
		}
	}
	while (true) {
		fileName = fss.nextFile();
		if (fileName.size() > 0) {
			if (fileName == nameString) {
				return searchPath + "/" + fileName;
			}
		}
		else
			break;
	}
	std::string dirName = fss.firstDirectory(searchPath);
	if (dirName.size() > 0) {
		if (dirName != "." && dirName != "..") {
			std::string retVal = searchForFile(
				nameString, searchPath + "/" + dirName);
			if (retVal.size() > 0)
				return retVal;
		}
	}
	while (true)
	{
		dirName = fss.nextDirectory();
		if (dirName.size() > 0) {
			std::cout << "\n  " << dirName;
			if (dirName != "." && dirName != "..") {
				std::string retVal = searchForFile(
					nameString, searchPath + "/" + dirName);
				if (retVal.size() > 0)
					return retVal;
			}
		}
		else
			break;
	}
	std::cout << std::endl;
	return "";
}

//----< see if a string contains a substring >----------------------------------

bool contains(std::string str, std::string sub) {
	int n = sub.size();
	for (int i = 0; i < str.size() - n + 1; i++) {
		if (str.substr(i, n) == sub) {
			return true;
		}
	}
	return false;
}

//----< create and store a new user's credentials >-----------------------------

int createNewUser(std::string& userName, std::string& password) {
	ifstream serverCredentialDataFileStream;
	serverCredentialDataFileStream.open("ServerCredentialData.txt");
	char buff[100];
	std::vector<std::vector<std::string>> allData;
	if (serverCredentialDataFileStream.is_open()) {
		while (!serverCredentialDataFileStream.eof()) {
			serverCredentialDataFileStream >> buff;
			std::string data = (std::string)buff;
			vector<string> strings;
			istringstream f(data);
			string s;
			while (getline(f, s, '$')) {
				strings.push_back(s);
			}
			if(strings.size() > 0)
				allData.push_back(strings);
			if (strings.size() > 0 && userName == strings[0]) {
				serverCredentialDataFileStream.close();
				return -1;
			}
		}
		ofstream newServerCredentialDataFileStream;
		newServerCredentialDataFileStream.open("ServerCredentialData.txt", 'w');
		for (std::vector<std::string> credential : allData) {
			newServerCredentialDataFileStream << credential[0] << "$" << 
				credential[1] << "$" << credential[2] << "\r\n";
		}
		newServerCredentialDataFileStream << userName.c_str() << "$" << 
			password.c_str() << "$" << "not_admin" << "\r\n";
		newServerCredentialDataFileStream.flush();
		newServerCredentialDataFileStream.close();
	}
	return -1;
}

//----< used for "debugging" >--------------------------------------------------

void loc(std::string s) {
	cout << s << endl;
}

//----< check the validity and status of given credentials >--------------------

std::string lookupCredentials(std::string userName, std::string password) {
	if (userName == "" || password == "")
		return "credential_not_good";
	ifstream serverCredentialDataFileStream;
	serverCredentialDataFileStream.open("ServerCredentialData.txt");
	char buff[100];
	ostringstream oss;
	if (serverCredentialDataFileStream.is_open()) {
		while (!serverCredentialDataFileStream.eof()) {
			serverCredentialDataFileStream >> buff;
			std::string data = (std::string)buff;
			vector<string> strings;
			istringstream f(data);
			string s;
			while (getline(f, s, '$')) {
				strings.push_back(s);
			}
			if (strings.size() > 0 && userName == strings[0] && 
				password == strings[1]) {
				serverCredentialDataFileStream.close();
				if (strings[2] == "is_admin") {
					return "credential_good_is_admin";
				}
				else {
					return "credential_good_not_admin";
				}
			}
		}
		serverCredentialDataFileStream.close();	
		return "credential_not_good";
	}
}

//----< get number of test requests so far associated with test logs files >----

int readInTestRequestIdFromFile() {
	ifstream serverProgramDataStream;
	serverProgramDataStream.open("ServerProgramData.txt");
	char buff[100];
	ostringstream oss;
	if (serverProgramDataStream.is_open()) {
		serverProgramDataStream >> buff;
		std::string data = (std::string)buff;
		int colonIndex = data.find_last_of(':');
		std::string testRequestIdString = data.substr(colonIndex + 1);
		return stoi(testRequestIdString);
	}
	cout << "Error reading ServerProgramData.txt" << endl;
	return -1;
}

//----< save number of testLogs saved so far - to avoid overwriting old ones >--

void saveNumberOfTestLogsSavedSoFar(int n) {
	ofstream serverProgramDataFileStream;
	serverProgramDataFileStream.open("ServerProgramData.txt", 'w');
	serverProgramDataFileStream << "total_number_of_test_logs:" << testRequestID;
	serverProgramDataFileStream.flush();
	serverProgramDataFileStream.close();
}

//----< get number of testLogs saved so far >-----------------------------------

int getNumberOfTestLogsSavedSoFar() {
	ifstream serverProgramDataFileStream;
	serverProgramDataFileStream.open("ServerProgramData.txt");
	char buff[100];
	if (serverProgramDataFileStream.is_open()) {
		while (!serverProgramDataFileStream.eof()) {
			serverProgramDataFileStream >> buff;
			std::string data = (std::string)buff;
			if (data.substr(0, 25) == "total_number_of_test_logs") {
				int index = data.find_last_of(":");
				std::string num = data.substr(index + 1);
				return stoi(num);
			}
		}
	}
	serverProgramDataFileStream.close();
	return 0;
}

//----< startup extractors and matchTestRequestsToReadyCTPs func >--------------

int main() {
	std::cout << "in server main" << endl << endl;
	testRequestID = readInTestRequestIdFromFile();
	spawn(1); spawn(2); 

	serverUserComm.start();
	serverCtpComm_1.start();
	serverCtpComm_2.start();
	serverCtpComm_3.start();

	std::thread t0(serverUserCommExtractor);
	std::thread t1(serverCtpCommExtractor_1);
	std::thread t2(serverCtpCommExtractor_2);
	std::thread t3(serverCtpCommExtractor_3);

	matchTestRequestsToReadyCTPs();

	serverUserComm.stop();
	serverCtpComm_1.stop();
	serverCtpComm_2.stop();
	serverCtpComm_3.stop();

	t0.join();
	t1.join();
	t2.join();
	t3.join();

	cin.get();
}

//----< route test requests to ready child tester processes >-------------------

void matchTestRequestsToReadyCTPs() {
	while (true) {
		Message testRequestMsg = testRequestBQ.deQ();
		Message readyMsg = readyMessageBQ.deQ();
		std::string readyID = readyMsg.name();
		int readyIDint = stoi(readyID);
		Message outgoing;
		ostringstream testRequestIdStream;
		testRequestID++;
		testRequestIdStream << testRequestID;  // for log file uniqueness
		outgoing.attribute("testRequestID", testRequestIdStream.str());
		saveNumberOfTestLogsSavedSoFar(testRequestID); // save to file
		switch (readyIDint) {
		case 3001:
			outgoing.to(ctpEPforServerCtpCommunication_1);
			outgoing.from(serverEPforServerCtpCommunication_1);
			outgoing.attribute("BODY", testRequestMsg.attributes().at("BODY"));
			outgoing.command("TEST_REQUEST");
			outgoing.name("7001");
			serverCtpComm_1.postMessage(outgoing);
			break;
		case 3002:
			outgoing.to(ctpEPforServerCtpCommunication_2);
			outgoing.from(serverEPforServerCtpCommunication_2);
			outgoing.attribute("BODY", testRequestMsg.attributes().at("BODY"));
			outgoing.command("TEST_REQUEST");
			outgoing.name("7002");
			serverCtpComm_2.postMessage(outgoing);
			break;
		case 3003:
			Message outgoing(ctpEPforServerCtpCommunication_3, 
				serverEPforServerCtpCommunication_3);
			outgoing.attribute("BODY", testRequestMsg.attributes().at("BODY"));
			outgoing.command("TEST_REQUEST");
			outgoing.name("7003");
			serverCtpComm_3.postMessage(outgoing);
			break;
		}
		ioMutex.lock();
		cout << "  --  testRequestCount: " << testRequestIdStream.str() << endl;
		ioMutex.unlock();
	}
}

//----< convert vector<string> into $ delimited string >------------------------

std::string convertVectorOfStringToString(std::vector<std::string> stringVec) {
	std::string result = "";
	for (std::string str : stringVec) {
		result += str + "$";
	}
	return result;
}

//////< Support serverUserCommExtractor >//////////////////////////////////////

//----< do credentials check >--------------------------------------------------

void doCredentialsCheck(Message msg) {
	std::string userName = msg.attributes().at("userName");
	std::string password = msg.attributes().at("password");
	std::string credentialCheck = lookupCredentials(userName, password);
	Message reply(msg.from(), msg.to());
	reply.command("check_credentials");
	reply.attribute("credentialCheck", credentialCheck);
	serverUserComm.postMessage(reply);
}

//----< create a new user >-----------------------------------------------------

void doCreateNewUser(Message msg) {
	std::string userName = msg.attributes().at("userName");
	std::string password = msg.attributes().at("password");
	createNewUser(userName, password);
}

//----< send file to specified location >---------------------------------------

void doDownloadFile(Message msg) {
	Message returnMsg(msg.from(), msg.to());
	returnMsg.command("FILE_MESSAGE");
	returnMsg.name("7000");
	sendFilePathTemp = sendFilePath;
	sendFilePath = msg.attributes().at("fromPath");
	returnMsg.file(msg.attributes().at("fileName")); // relative paths
	serverUserComm.postMessage(returnMsg);
}

//----< receive file transfer messages >----------------------------------------

void doUploadFile(Message msg) {
	if (msg.contentLength() == 0) {
		saveFilePath = saveFilePathTemp;
		Message xferCompleteMsg(aUserEPforServerUserCommunication, msg.to());
		xferCompleteMsg.command("File_Upload_Complete");
		serverUserComm.postMessage(xferCompleteMsg);
	}
}

//----< search Mock Repo for a specified file >---------------------------------

void doSearchForFile(Message msg) {
	std::string result = searchForFile(
		msg.attributes().at("searchFile"), "../MockRepo");
	if (result.size() == 0)
		result = "File Not Found";
	msg.attribute("fileSearchResult", result);
	msg.command("File_Search_Reply");
	msg.to(aUserEPforServerUserCommunication);
	serverUserComm.postMessage(msg);
}

//----< search Mock Repo for a specified directory >----------------------------

void doSearchForDir(Message msg) {
	std::string result = searchForDirectory(
		msg.attributes().at("searchDirName"), "../MockRepo");
	if (result.size() == 0)
		result = "Directory Not Found";
	msg.attribute("dirSearchResult", result);
	msg.command("Dir_Search_Reply");
	msg.to(aUserEPforServerUserCommunication);
	serverUserComm.postMessage(msg);
}

//----< create new directory in Mock Repository >------------------------------

void doMakeDir(Message msg) {
	FileSystem::Directory fsd;
	std::string dirName = msg.attributes().at("dir_name");
	dirName = "../MockRepo/" + dirName;
	fsd.create(dirName);
}

//----< show contents of a directory, used in GUI >-----------------------------

void doShowContents(Message msg) {
	FileSystem::Directory fsd;
	std::string dirName = msg.attributes().at("dir_name");
	std::vector<std::string> dirs = fsd.getDirectories(dirName);
	for (int i = 0; i < dirs.size(); i++) {
		if (dirs[i] != "." && dirs[i] != "..")
			dirs[i] = "<dir> " + dirs[i];
	}
	std::vector<std::string> files = fsd.getFiles(dirName);
	std::vector<std::string> contents = dirs;
	contents.insert(contents.end(), files.begin(), files.end());
	std::string contentsString = convertVectorOfStringToString(contents);
	Message replyMsg(
		aUserEPforServerUserCommunication, serverEPforServerUserCommunication);
	replyMsg.command("Show_Contents_Reply");
	replyMsg.name("7000");
	replyMsg.attribute("Contents_String", contentsString);
	serverUserComm.postMessage(replyMsg);
}

//----< handle messages from user >---------------------------------------------

void serverUserCommExtractor() {
	while (true) {
		Message msg = serverUserComm.getMessage();
		ioMutex.lock();
		if (msg.command() == "TEST_REQUEST") {
			testRequestBQ.enQ(msg);
		}
		else if (msg.command() == "stop") {
			saveNumberOfTestLogsSavedSoFar(testRequestID);
		}
		else if (msg.command() == "check_credentials") {
			doCredentialsCheck(msg);
		}
		else if (msg.command() == "create_new_user") {
			doCreateNewUser(msg);
		}
		else if (msg.command() == "Download_File") {
			doDownloadFile(msg);
		}
		else if (msg.command() == "File_Download_Complete") {
			sendFilePath = sendFilePathTemp;
		}
		else if (msg.command() == "Change_Save_File_Path") {
			saveFilePathTemp = saveFilePath;
			saveFilePath = msg.attributes().at("toPath");
		}
		else if (msg.command() == "Upload_File") {
			doUploadFile(msg);
		}
		else if (msg.command() == "Search_For_File") {
			doSearchForFile(msg);
		}
		else if (msg.command() == "Search_For_Dir") {
			doSearchForDir(msg);
		}
		else if (msg.command() == "Make_Dir") {
			doMakeDir(msg);
		}
		else if (msg.command() == "Show_Contents") {
			doShowContents(msg);
		}
		ioMutex.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(
			contrivedExtractorDelayTime));
	}
}

//----< enqueue ready messages from child tester process 1 >--------------------

void serverCtpCommExtractor_1() {
	while (true) {
		Message msg = serverCtpComm_1.getMessage();
		ioMutex.lock();
		if (msg.command() == "READY_MESSAGE") {
			readyMessageBQ.enQ(msg);
		}
		ioMutex.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(
			contrivedExtractorDelayTime));
	}
}

//----< enqueue ready messages from child tester process 2 >--------------------

void serverCtpCommExtractor_2() {
	while (true) {
		Message msg = serverCtpComm_2.getMessage();
		ioMutex.lock();
		if (msg.command() == "READY_MESSAGE") {
			readyMessageBQ.enQ(msg);
		}
		ioMutex.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(
			contrivedExtractorDelayTime));
	}
}

//----< enqueue ready messages from child tester process 3 >--------------------

void serverCtpCommExtractor_3() {
	while (true) {
		Message msg = serverCtpComm_3.getMessage();
		ioMutex.lock();
		if (msg.command() == "READY_MESSAGE") {
			readyMessageBQ.enQ(msg);
		}
		ioMutex.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(
			contrivedExtractorDelayTime));
	}
}

//----< spawn a child process with id to run tests in a loop >------------------

int spawn(int id) {		// squished down to <= 50 lines
	ostringstream oss;
	oss << id << " I_CHANGE_THE_ARGUMENT_COUNT";
	std::string str = oss.str();
	char *pCL = &str[0];
	LPCTSTR appName_ = "../Debug/ChildTesterProcess.exe";
	STARTUPINFO startInfo;          // establishes properties of child process
	PROCESS_INFORMATION procInfo;   // holds info returned from CreateProcess
	LPCTSTR appName = appName_;            // program to run
	LPTSTR  commandLine = pCL;                // main's args
	LPSECURITY_ATTRIBUTES procSec = NULL;               // default security
	LPSECURITY_ATTRIBUTES thrdSec = NULL;               // default security
	BOOL inheritHandles = FALSE;              // don't inherit hndls
	DWORD createFlags = CREATE_NEW_CONSOLE; // make a console
	LPVOID environment = NULL;               // caller's evironmt
	LPCTSTR currentPath = NULL;               // default dir
	LPSTARTUPINFO pStartInfo = &startInfo;         // set process style
	LPPROCESS_INFORMATION pPrInfo = &procInfo;          // retrieve info
	string buildTitle = string("child process: ") += appName_;
	GetStartupInfo(&startInfo);
	startInfo.lpTitle = (char*)buildTitle.c_str();
	startInfo.dwFlags = STARTF_USEPOSITION | STARTF_USESIZE;
	startInfo.dwX = 200;	  startInfo.dwY = 250;
	startInfo.dwXSize = 600;  startInfo.dwYSize = 300;
	BOOL OK =
		CreateProcess( appName, commandLine, procSec, thrdSec, inheritHandles,
					createFlags, environment, currentPath, pStartInfo, pPrInfo );
	CloseHandle(procInfo.hThread);  // parent needs no access to child thread
	if (!OK) {
		cout << "\n failed to create process: " << appName << "\n\n";
		DWORD errorCode = GetLastError();
		DWORD dwFlags =
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER;
		LPCVOID lpSource = NULL;		DWORD dwMessageID = errorCode;
		DWORD dwLanguageId = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);
		LPSTR lpBuffer;		DWORD nSize = 0;	va_list *Arguments = NULL;
		FormatMessage(
			dwFlags, lpSource, dwMessageID, dwLanguageId,
			(LPTSTR)&lpBuffer, nSize, Arguments
		);
		cout << "\n  error: ";
		if (lpBuffer)
			cout << lpBuffer << "\n  --- did you forget extension .exe?" << "\n\n";
		return 1;
	}
	CloseHandle(procInfo.hProcess);
	return 0;
}

/////////////////////////////////////////////////////////////////////

// All original code by Dr. Fawcett below this point

/////////////////////////////////////////////////////////////////////

//----< constructor sets port >--------------------------------------

Receiver::Receiver(EndPoint ep, const std::string& name) : listener(ep.port), rcvrName(name)
{
  StaticLogger<1>::write("\n  -- starting Receiver");
}
//----< returns reference to receive queue >-------------------------

BlockingQueue<Message>* Receiver::queue()
{
  return &rcvQ;
}
//----< starts listener thread running callable object >-------------

template<typename CallableObject>
void Receiver::start(CallableObject& co)
{
  listener.start(co);
}
//----< stops listener thread >--------------------------------------

void Receiver::stop()
{
  listener.stop();
}
//----< retrieves received message >---------------------------------

Message Receiver::getMessage()
{
  StaticLogger<1>::write("\n  -- " + rcvrName + " deQing message");
  return rcvQ.deQ();
}
//----< constructor initializes endpoint object >--------------------

Sender::Sender(const std::string& name) : sndrName(name)
{
  lastEP = EndPoint();  // used to detect change in destination
}
//----< destructor waits for send thread to terminate >--------------

Sender::~Sender()
{
  if (sendThread.joinable())
    sendThread.join();
}
//----< starts send thread deQ, inspect, and send loop >-------------

void Sender::start()
{
  std::function <void()> threadProc = [&]() {
    while (true)
    {
      Message msg = sndQ.deQ();

      if (msg.command() == "quit")
      {
        StaticLogger<1>::write("\n  -- send thread shutting down");
        return;
      }
      StaticLogger<1>::write("\n  -- " + sndrName + " send thread sending " + msg.name());
      std::string msgStr = msg.toString();

      if (msg.to().address != lastEP.address || msg.to().port != lastEP.port)
      {
        connecter.shutDown();
        //connecter.close();
        StaticLogger<1>::write("\n  -- attempting to connect to new endpoint: " + msg.to().toString());
        if (!connect(msg.to()))
        {
          StaticLogger<1>::write("\n can't connect");
          continue;
        }
        else
        {
          StaticLogger<1>::write("\n  connected to " + msg.to().toString());
        }
      }
      if (msg.containsKey("file"))
      {
        sendFile(msg);
      }
      else
      {
        bool sendRslt = connecter.send(msgStr.length(), (Socket::byte*)msgStr.c_str());
      }
    }
  };
  std::thread t(threadProc);
  sendThread = std::move(t);
}
//----< stops send thread by posting quit message >------------------

void Sender::stop()
{
  Message msg;
  msg.name("quit");
  msg.command("quit");
  postMessage(msg);
  connecter.shutDown();
}
//----< attempts to connect to endpoint ep >-------------------------

bool Sender::connect(EndPoint ep)
{
  lastEP = ep;
  return connecter.connect(ep.address, ep.port);
}
//----< posts message to send queue >--------------------------------

void Sender::postMessage(Message msg)
{
  sndQ.enQ(msg);
}
//----< sends binary file >------------------------------------------
/*
*  - not implemented yet
*/
bool Sender::sendFile(Message msg)
{
  if (!msg.containsKey("file"))
    return false;
  std::string fileSpec = sendFilePath + "/" + msg.file();
  std::ifstream sendFile(fileSpec, std::ios::binary);
  if (!sendFile.good())
    return false;
  while (true)
  {
    sendFile.read(rwBuffer, BlockSize);
    size_t blockSize = (size_t)sendFile.gcount();
    msg.contentLength(blockSize);
    std::string msgString = msg.toString();
    connecter.sendString(msgString);
    if (blockSize == 0)
      break;
    connecter.send(blockSize, rwBuffer);
  }
  sendFile.close();
  return true;
}
//----< callable object posts incoming message to rcvQ >-------------
/*
*  This is ClientHandler for receiving messages and posting
*  to the receive queue.
*/
class ClientHandler
{
public:
  //----< acquire reference to shared rcvQ >-------------------------

  ClientHandler(BlockingQueue<Message>* pQ, const std::string& name = "clientHandler") : pQ_(pQ), clientHandlerName(name)
  {
    StaticLogger<1>::write("\n  -- starting ClientHandler");
  }
  //----< shutdown message >-----------------------------------------

  ~ClientHandler() 
  { 
    StaticLogger<1>::write("\n  -- ClientHandler destroyed;"); 
  }
  //----< set BlockingQueue >----------------------------------------

  void setQueue(BlockingQueue<Message>* pQ)
  {
    pQ_ = pQ;
  }
  //----< frame message string by reading bytes from socket >--------

  std::string readMsg(Socket& socket)
  {
    std::string temp, msgString;
    while (socket.validState())
    {
      temp = socket.recvString('\n');  // read attribute
      msgString += temp;
      if (temp.length() < 2)           // if empty line we are done
        break;
    }
    return msgString;
  }
  //----< receive file blocks >--------------------------------------
  /*
  *  - expects msg to contain file and contentLength attributes
  *  - expects to be connected to appropriate destination
  *  - these requirements are established in Sender::start()
  */
  bool receiveFile(Message msg)
  {
    std::string fileName = msg.file();
    std::string fileSpec = saveFilePath + "/" + fileName;
    std::ofstream saveStream(fileSpec, std::ios::binary);
    if (!saveStream.good())
    {
      return false;
    }
    while (true)
    {
      size_t blockSize = msg.contentLength();
      if (blockSize == 0)
        break;
      Socket::byte terminator;
      pSocket->recv(1, &terminator);
      pSocket->recv(blockSize, rwBuffer);
      saveStream.write(rwBuffer, blockSize);
      std::string msgString = readMsg(*pSocket);
      if (msgString.length() == 0)
      {
        break;
      }
      msg = Message::fromString(msgString);
      if (msg.contentLength() == 0)
        break;
    }
    saveStream.flush();
    saveStream.close();
    pQ_->enQ(msg);
    return true;
  }
  //----< reads messages from socket and enQs in rcvQ >--------------

  void operator()(Socket socket)
  {
    pSocket = &socket;
    while (socket.validState())
    {
      std::string msgString = readMsg(socket);
      if (msgString.length() == 0)
      {
        // invalid message
        break;
      }
      Message msg = Message::fromString(msgString);
      StaticLogger<1>::write("\n  -- " + clientHandlerName + " RecvThread read message: " + msg.name());
      //std::cout << "\n  -- " + clientHandlerName + " RecvThread read message: " + msg.name();
      if (msg.containsKey("file"))
      {
        receiveFile(msg);
      }
      pQ_->enQ(msg);
      //std::cout << "\n  -- message enqueued in rcvQ";
      if (msg.command() == "quit")
        break;
    }
    StaticLogger<1>::write("\n  -- terminating ClientHandler thread");
  }
private:
  BlockingQueue<Message>* pQ_;
  std::string clientHandlerName;
  Socket* pSocket = nullptr;
};

Comm::Comm(EndPoint ep, const std::string& name) : rcvr(ep, name), sndr(name), commName(name) {}

void Comm::start()
{
  BlockingQueue<Message>* pQ = rcvr.queue();
  ClientHandler* pCh = new ClientHandler(pQ, commName);
  /*
    There is a trivial memory leak here.  
    This ClientHandler is a prototype used to make ClientHandler copies for each connection.
    Those are not created on the heap, and are destroyed when the connection closes.
    Only one Client handler prototype is created for each Comm object and will live until
    the end of the program.

    I will clean this up in the next version.
  */
  rcvr.start(*pCh);
  sndr.start();
}

void Comm::stop()
{
  rcvr.stop();
  sndr.stop();
}

void Comm::postMessage(Message msg)
{
  sndr.postMessage(msg);
}

Message Comm::getMessage()
{
  return rcvr.getMessage();
}

std::string Comm::name()
{
  return commName;
}

//----< test stub >--------------------------------------------------

#ifdef TEST_COMM

/////////////////////////////////////////////////////////////////////
// Test #1 - Demonstrates Sender and Receiver operations

void DemoSndrRcvr(const std::string& machineName)
{
  SUtils::title("Demonstrating Sender and Receiver classes");

  SocketSystem ss;
  EndPoint ep1;
  ep1.port = 9091;
  ep1.address = "localhost";
  Receiver rcvr1(ep1);
  BlockingQueue<Message>* pQ1 = rcvr1.queue();

  ClientHandler ch1(pQ1);
  rcvr1.start(ch1);

  EndPoint ep2;
  ep2.port = 9092;
  ep2.address = "localhost";
  Receiver rcvr2(ep2);
  BlockingQueue<Message>* pQ2 = rcvr2.queue();

  ClientHandler ch2(pQ2);
  rcvr2.start(ch2);

  Sender sndr;
  sndr.start();
  bool connected = sndr.connect(ep1);
  Message msg;
  msg.name("msg #1");
  msg.to(ep1);
  msg.from(msg.to());
  msg.command("do it");
  msg.attribute("bodyAttrib", "zzz");
  StaticLogger<1>::flush();
  std::cout << "\n  sndr in main posting message:  " << msg.name();
  sndr.postMessage(msg);

  msg.name("msg #2");
  msg.to(EndPoint(machineName, 9092));
  StaticLogger<1>::flush();
  std::cout << "\n  sndr in main posting message:  " << msg.name();
  sndr.postMessage(msg);

  Message rcvdMsg = rcvr1.getMessage();  // blocks until message arrives
  StaticLogger<1>::flush();
  std::cout << "\n  rcvr1 in main received message: " << rcvdMsg.name();
  rcvdMsg.show();

  rcvdMsg = rcvr2.getMessage();  // blocks until message arrives
  StaticLogger<1>::flush();
  std::cout << "\n  rcvr2 in main received message: " << rcvdMsg.name();
  rcvdMsg.show();

  SUtils::title("Sending message to EndPoint that doesn't exist");

  msg.name("msg #3");
  msg.to(EndPoint("DoesNotExist", 1111));  // Unknown endpoint - should fail
  StaticLogger<1>::flush();
  std::cout << "\n  sndr in main posting message:  " << msg.name();
  msg.show();
  sndr.postMessage(msg);                   // will never reach rcvr

  msg.name("msg #4");
  msg.to(EndPoint("localhost", 9091));
  StaticLogger<1>::flush();
  std::cout << "\n  sndr in main posting message:  " << msg.name();
  sndr.postMessage(msg);                  // this should succeed
  StaticLogger<1>::flush();
  rcvdMsg = rcvr1.getMessage();
  std::cout << "\n  rcvr1 in main received message: " << rcvdMsg.name();
  rcvdMsg.show();

  rcvr1.stop();
  rcvr2.stop();
  sndr.stop();
  StaticLogger<1>::flush();

  std::cout << "\n  press enter to quit DemoSndrRcvr";
  _getche();
  std::cout << "\n";
}

/////////////////////////////////////////////////////////////////////
// Test #2 - Demonstrates Comm class using a single thread
//           sending and receiving messages from two Comm
//           instances.

void DemoCommClass(const std::string& machineName)
{
  SUtils::title("Demonstrating Comm class");

  SocketSystem ss;

  EndPoint ep1("localhost", 9191);
  Comm comm1(ep1, "comm1");
  comm1.start();

  EndPoint ep2("localhost", 9192);
  Comm comm2(ep2, "comm2");
  comm2.start();

  // send msg from comm1 to comm1
  Message msg;
  msg.name("msg #1");
  msg.to(ep1);
  msg.from(ep1);
  StaticLogger<1>::flush();
  std::cout << "\n  comm1 in main posting message:   " << msg.name();
  comm1.postMessage(msg);
  msg = comm1.getMessage();
  StaticLogger<1>::flush();
  std::cout << "\n  comm1 in main received message:  " << msg.name();
  msg.show();

  // send msg from comm1 to comm2
  msg.name("msg #2");
  msg.from(ep1);
  msg.to(ep2);
  StaticLogger<1>::flush();
  std::cout << "\n  comm1 in main posting message:  " << msg.name();
  comm1.postMessage(msg);
  msg = comm2.getMessage();
  StaticLogger<1>::flush();
  std::cout << "\n  comm2 in main received message: " << msg.name();
  msg.show();

  // send msg from comm2 to comm1
  msg.name("msg #3");
  msg.to(ep1);
  msg.from(ep2);
  StaticLogger<1>::flush();
  std::cout << "\n  comm2 in main posting message:  " << msg.name();
  comm2.postMessage(msg);
  msg = comm1.getMessage();
  StaticLogger<1>::flush();
  std::cout << "\n  comm1 in main received message: " << msg.name();
  msg.show();

  // send msg from comm2 to comm2
  msg.name("msg #4");
  msg.from(ep2);
  msg.to(ep2);
  StaticLogger<1>::flush();
  std::cout << "\n  comm2 in main posting message:  " << msg.name();
  comm2.postMessage(msg);
  msg = comm2.getMessage();
  StaticLogger<1>::flush();
  std::cout << "\n  comm2 in main received message: " << msg.name();
  msg.show();

  comm1.stop();
  comm2.stop();
  StaticLogger<1>::flush();
  std::cout << "\n  press enter to quit DemoComm";
  _getche();
}
/////////////////////////////////////////////////////////////////////
// Test #3 - Demonstrate server with two concurrent clients
//           sending and receiving messages

//----< handler for first concurrent client >------------------------

void ThreadProcClnt1()
{
  Comm comm(EndPoint("localhost", 9891), "client1Comm");
  comm.start();
  EndPoint serverEP("localhost", 9890);
  EndPoint clientEP("localhost", 9891);
  size_t IMax = 3;
  for (size_t i = 0; i < IMax; ++i)
  {
    Message msg(serverEP, clientEP);
    msg.name("client #1 : msg #" + Utilities::Converter<size_t>::toString(i));
    std::cout << "\n  " + comm.name() + " posting:  " << msg.name();
    comm.postMessage(msg);
    Message rply = comm.getMessage();
    std::cout << "\n  " + comm.name() + " received: " << rply.name();
    ::Sleep(100);
  }
  Message fileMsg(serverEP, clientEP);
  fileMsg.name("fileSender");
  fileMsg.file("logger.cpp");
  comm.postMessage(fileMsg);
  ::Sleep(200);

  Message stop;
  stop.name("stop");
  stop.to(serverEP);
  stop.command("stop");
  comm.postMessage(stop);
}
//----< handler for 2nd concurrent client >--------------------------

void ThreadProcClnt2()
{
  Comm comm(EndPoint("localhost", 9892), "client2Comm");
  comm.start();
  EndPoint serverEP("localhost", 9890);
  EndPoint clientEP("localhost", 9892);
  size_t IMax = 3;
  for (size_t i = 0; i < IMax; ++i)
  {
    Message msg(serverEP, clientEP);
    msg.name("client #2 : msg #" + Utilities::Converter<size_t>::toString(i));
    std::cout << "\n  " + comm.name() + " posting:  " << msg.name();
    comm.postMessage(msg);
    Message rply = comm.getMessage();
    std::cout << "\n  " + comm.name() + " received: " << rply.name();
  }
  Message fileMsg(serverEP, clientEP);
  fileMsg.name("fileSender");
  fileMsg.file("logger.h");
  comm.postMessage(fileMsg);
  ::Sleep(200);
}
//----< server demonstrates two-way asynchronous communication >-----
/*
*  - One server receiving messages and sending replies to
*    two concurrent clients.
*/



void DemoClientServer()
{
  SUtils::title("Demonstrating Client-Server - one server with two concurrent clients");

  SocketSystem ss;

  EndPoint serverEP("localhost", 9890);
  EndPoint clientEP("localhost", 9891);
  Comm comm(serverEP, "serverComm");
  comm.start();
  std::thread t1(ThreadProcClnt1);
  t1.detach();
  std::thread t2(ThreadProcClnt2);
  t2.detach();

  Message msg, rply;
  rply.name("reply");
  size_t count = 0;
  while (true)
  {
    // display each incoming message

    msg = comm.getMessage();
    std::cout << "\n  " + comm.name() + " received message: " << msg.name();

    if (msg.containsKey("file"))  // is this a file message?
    {
      if(msg.contentLength() == 0)
        std::cout << "\n  " + comm.name() + " received file \"" + msg.file() + "\" from " + msg.name();
    }
    else  // non-file message
    {
      rply.to(msg.from());
      rply.from(serverEP);
      rply.name("server reply #" + Utilities::Converter<size_t>::toString(++count) + " to " + msg.from().toString());

      comm.postMessage(rply);
      if (msg.command() == "stop")
      {
        break;
      }
    }
  }

  comm.stop();
  StaticLogger<1>::flush();
  std::cout << "\n  press enter to quit DemoClientServer";
  _getche();
}

Cosmetic cosmetic;


#endif
