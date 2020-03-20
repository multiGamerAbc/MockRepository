///////////////////////////////////////////////////////////////////////////
// Comm.h - message-passing communication facility                       //
// ver 2.0 (Modified for CSE687 OOD Project 4)                           //
// Author: Phil Maalouf, pmaalouf@syr.edu                                //
// Source: Jim Fawcett, CSE687-OnLine Object Oriented Design, Fall 2017  //
///////////////////////////////////////////////////////////////////////////
#include <algorithm>
#include "CommUser.h"
#include <conio.h>
#include <exception>
#include "FileManager.h"
#include <fstream>
#include <functional>
#include <iostream>
#include <shared_mutex>
#include <string>
#include "TestHarness.h"
#include <vector>

#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "../FileSystemDemo/FileSystem.h"
#include "../Logger/Logger.h"
#include "../TestHarnessUtilities/MyLogger.h"
#include "../Utilities/Utilities.h"

//----< variables (set 1) >-----------------------------------------------------

using namespace MsgPassingCommunication;
using namespace Sockets;
using SUtils = Utilities::StringHelper;
std::string saveFilePath = "../SaveFiles";
std::string sendFilePath = "../SendFiles";
const size_t BlockSize = 1024;
Socket::byte rwBuffer[BlockSize];

//----< variables (set 2) >-----------------------------------------------------

std::string tempSaveFilePath = "";
std::string tempSendFilePath = "";
int contrivedExtractorDelayTime = 20;
std::mutex ioMutex;
std::shared_mutex fileMutex;
BlockingQueue<Message> fileRequestBQ_1;
BlockingQueue<Message> fileRequestBQ_2;
BlockingQueue<Message> fileRequestBQ_3;
SocketSystem ss;

int aUserPortNumForUserGuiCommunication = 9000;
int aUserPortNumForUserServerCommunication = 5000;
int aUserPortNumForUserCtpCommunication_1 = 5001;
int aUserPortNumForUserCtpCommunication_2 = 5002;
int aUserPortNumForUserCtpCommunication_3 = 5003;

int guiPortNumForUserGuiCommunication = 9001;
int serverPortNumForUserServerCommunication = 7000;
int ctpPortNumForUserCtpCommunication_1 = 3501;
int ctpPortNumForUserCtpCommunication_2 = 3502;
int ctpPortNumForUserCtpCommunication_3 = 3503;

EndPoint aUserEPforUserGuiCommunication(
	"localhost", aUserPortNumForUserGuiCommunication);
EndPoint aUserEPforUserServerCommunication(
	"localhost", aUserPortNumForUserServerCommunication);
EndPoint aUserEPforUserCtpCommunication_1(
	"localhost", aUserPortNumForUserCtpCommunication_1);
EndPoint aUserEPforUserCtpCommunication_2(
	"localhost", aUserPortNumForUserCtpCommunication_2);
EndPoint aUserEPforUserCtpCommunication_3(
	"localhost", aUserPortNumForUserCtpCommunication_3);

EndPoint guiEPforUserGuiCommunication(
	"localhost", guiPortNumForUserGuiCommunication);
EndPoint serverEPforUserServerCommunication(
	"localhost", serverPortNumForUserServerCommunication);
EndPoint ctpEPforUserCtpCommunication_1(
	"localhost", ctpPortNumForUserCtpCommunication_1);
EndPoint ctpEPforUserCtpCommunication_2(
	"localhost", ctpPortNumForUserCtpCommunication_2);
EndPoint ctpEPforUserCtpCommunication_3(
	"localhost", ctpPortNumForUserCtpCommunication_3);

Comm userGuiComm(aUserEPforUserGuiCommunication, "userGuiComm");
Comm userServerComm(aUserEPforUserServerCommunication, "userServerComm");
Comm userCtpComm_1(aUserEPforUserCtpCommunication_1, "userCtpComm");
Comm userCtpComm_2(aUserEPforUserCtpCommunication_2, "userCtpComm");
Comm userCtpComm_3(aUserEPforUserCtpCommunication_3, "userCtpComm");

//----< function declarations >-------------------------------------------------

void userCtpCommExtractor_1();
void userCtpCommExtractor_2();
void userCtpCommExtractor_3();
void sendTestRequestMessagesToServer();
void fileRequestBQ_1Extractor();
void fileRequestBQ_2Extractor();
void fileRequestBQ_3Extractor();
void userGuiCommListener();
void relayTestRequestMessageToServer(Message msg);
void sendTenTimesNTestRequestMessagesToServer(int n);
void userServerCommListener();

//----< sends test requests and grants file requests >--------------------------

int main() {
	std::cout << "in user main" << endl << endl;
	cout << "Demonstrate Requirment #4 Items (from Project 3, I left it in)\n";
	Message demoMsg;
	demoMsg.to(aUserEPforUserServerCommunication);
	demoMsg.from(serverEPforUserServerCommunication);
	demoMsg.command("SOME_MESSAGE_TYPE");
	demoMsg.name("Author name, given as port number throughout program");
	demoMsg.attribute("BODY", "<TestElement>"
		"<TestDriverElement>SomeTestDriverName</TestDriverElement>"
		"<TestedCodeElement>TestedCodeElement1</TestedCodeElement>"
		"<TestedCodeElement>TestedCodeElement2</TestedCodeElement>"
		"</TestElement>");
	cout << "demoMsg.to()   (destination address):  " << 
		demoMsg.to().toString() << endl;
	cout << "demoMsg.from()      (source address):  " << 
		demoMsg.from().toString() << endl;
	cout << "demoMsg.command()     (Message Type):  " << 
		demoMsg.command() << endl;
	cout << "demoMsg.name()              (Author):  " << 
		demoMsg.name() << endl;
	cout << "demoMsg.timeDate():                    " << 
		demoMsg.timeDate();
	cout << "demoMsg.attributes().at(\"BODY\"):     " << 
		demoMsg.attributes().at("BODY") << endl << endl;
	userGuiComm.start();
	userServerComm.start();
	userCtpComm_1.start();
	userCtpComm_2.start();
	userCtpComm_3.start();
	std::thread t1(userCtpCommExtractor_1);
	std::thread t2(userCtpCommExtractor_2);
	std::thread t3(userCtpCommExtractor_3);
	std::thread t4(fileRequestBQ_1Extractor);
	std::thread t5(fileRequestBQ_2Extractor);
	std::thread t6(fileRequestBQ_3Extractor);
	std::thread t7(userGuiCommListener);
	std::thread t8(userServerCommListener);

	t1.join(); t2.join(); t3.join(); t4.join();
	t5.join(); t6.join(); t7.join(); t8.join();
	cin.get();
	userGuiComm.stop();
	userServerComm.stop();
	userCtpComm_1.stop();
	userCtpComm_2.stop();
	userCtpComm_3.stop();
}

//----< stop the server >-------------------------------------------------------

void stopServer() {
	Message msg(
		serverEPforUserServerCommunication, aUserEPforUserServerCommunication);
	msg.command("stop");
	userServerComm.postMessage(msg);
	// implement: join all threads and stop all Comms in the user
}

//----< convert vector<string> into $ delimited string >------------------------
// Used in CommUser.cpp?
std::string convertVectorOfStringToString(std::vector<std::string> stringVec) {
	std::string result = "";
	for (std::string str : stringVec) {
		result += str + "$";
	}
	return result;
}

//----< extract and process messages from userServerComm >----------------------

void userServerCommListener() {
	while (true) {
		Message msg = userServerComm.getMessage();
		if (msg.command() == "check_credentials") {
			Message toRelay(
				guiEPforUserGuiCommunication, aUserEPforUserGuiCommunication);
			toRelay.command("check_credentials");
			toRelay.attribute(
				"credentialCheck", msg.attributes().at("credentialCheck"));
			userGuiComm.postMessage(toRelay);
		}
		else if (msg.command() == "FILE_MESSAGE") {
			if (msg.contentLength() == 0) {
				saveFilePath = tempSaveFilePath;
				Message xferCompleteMsg(msg.from(), msg.to());
				xferCompleteMsg.command("File_Download_Complete");
				userServerComm.postMessage(xferCompleteMsg);
			}
		}
		else if (msg.command() == "File_Upload_Complete") {
			sendFilePath = tempSendFilePath;
		}
		else if (msg.command() == "File_Search_Reply" || 
			msg.command() == "Dir_Search_Reply"       ||
			msg.command() == "Show_Contents_Reply") {
			msg.to(guiEPforUserGuiCommunication);
			userGuiComm.postMessage(msg);
		}
	}
}

//----< do file transfer to server >--------------------------------------------

void doFileUpload(Message& msg) {
	tempSendFilePath = sendFilePath;
	sendFilePath = msg.attributes().at("fromPath");
	Message changeSaveFilePathCommand(
		serverEPforUserServerCommunication, aUserEPforUserServerCommunication);
	changeSaveFilePathCommand.command("Change_Save_File_Path");
	changeSaveFilePathCommand.attribute(
		"toPath", msg.attributes().at("toPath"));
	userServerComm.postMessage(changeSaveFilePathCommand);
	msg.to(serverEPforUserServerCommunication);
	msg.from(aUserEPforUserServerCommunication);
	msg.file(msg.attributes().at("fileName"));
	userServerComm.postMessage(msg);
}

//----< show contents of a directory, used in GUI >-----------------------------

void doShowContents(Message& msg) {
	msg.to(serverEPforUserServerCommunication);
	userServerComm.postMessage(msg);
}

//----< create a new directory in the Mock Repository >-------------------------

void doMakeDir(Message& msg) {
	msg.to(serverEPforUserServerCommunication);
	userServerComm.postMessage(msg);
}

//----< relay a test request Message to server >--------------------------------

void relayTestRequestMessageToServer(Message msg) {
	Message testRequestMsg(
		serverEPforUserServerCommunication, aUserEPforUserServerCommunication);
	testRequestMsg.command("TEST_REQUEST");
	testRequestMsg.name("5000");
	testRequestMsg.attribute("BODY", msg.name());
	userServerComm.postMessage(testRequestMsg);
}

//----< have the server do a credential check >---------------------------------

void doCredentialCheck(Message msg) {
	Message toRelay(serverEPforUserServerCommunication,
		aUserEPforUserServerCommunication);
	toRelay.command("check_credentials");
	toRelay.attribute("userName", msg.attributes().at("userName"));
	toRelay.attribute("password", msg.attributes().at("password"));
	userServerComm.postMessage(toRelay);
}

//----< have the server create a new user >-------------------------------------

void doCreateNewUser(Message msg) {
	Message toRelay(serverEPforUserServerCommunication,
		aUserEPforUserServerCommunication);
	toRelay.command("create_new_user");
	toRelay.attribute("userName", msg.attributes().at("userName"));
	toRelay.attribute("password", msg.attributes().at("password"));
	userServerComm.postMessage(toRelay);
}

//----< request a file download from the server >-------------------------------

void requestFileDownload(Message msg) {
	tempSaveFilePath = saveFilePath;
	saveFilePath = msg.attributes().at("toPath");
	msg.to(serverEPforUserServerCommunication);
	msg.from(aUserEPforUserServerCommunication);
	userServerComm.postMessage(msg);
}

//----< extract and process messages from userGuiComm >-------------------------

void userGuiCommListener() {
	while (true) {
		Message msg = userGuiComm.getMessage();
		if (msg.command() == "Run_A_Test") {
			sendTestRequestMessagesToServer();
		}
		else if (msg.command() == "Test_Request") {
			relayTestRequestMessageToServer(msg);
		}
		else if (msg.command() == "Make_Dir") {
			doMakeDir(msg);
		}
		else if (msg.command() == "Show_Contents") {
			doShowContents(msg);
		}
		else if (msg.command() == "stop") {
			stopServer();
		}
		else if (msg.command() == "check_credentials") {
			doCredentialCheck(msg);
		}
		else if (msg.command() == "create_new_user") {
			doCreateNewUser(msg);
		}
		else if (msg.command() == "Download_File") {
			requestFileDownload(msg);
		}
		else if (msg.command() == "Upload_File") {
			doFileUpload(msg);
		}
		else if (msg.command() == "Search_For_File" || 
			msg.command() == "Search_For_Dir") {
			msg.to(serverEPforUserServerCommunication);
			userServerComm.postMessage(msg);
		}
	}
}

//----< extract and process messages from userCtpComm_1 >----------------------

void userCtpCommExtractor_1() {
	while (true) {
		Message msg = userCtpComm_1.getMessage();
		ioMutex.lock();
		ioMutex.unlock();
		if (msg.command() == "FILE_REQUEST") {
			fileRequestBQ_1.enQ(msg);
		}
		else if (msg.command() == "FILE_COMPLETE") {
			fileMutex.unlock();
		}
		std::this_thread::sleep_for(
			std::chrono::milliseconds(contrivedExtractorDelayTime));
	}
}

//----< extract and process messages from fileRequestBQ_1 >-------------------

void fileRequestBQ_1Extractor() {
	while (true) {
		Message msg = fileRequestBQ_1.deQ();
		Message returnMsg(
			ctpEPforUserCtpCommunication_1, aUserEPforUserCtpCommunication_1);
		returnMsg.command("FILE_MESSAGE");
		returnMsg.name("5001");
		returnMsg.file(msg.attributes().at("file_requested"));
		fileMutex.lock();
		userCtpComm_1.postMessage(returnMsg);
	}
}

//----< extract and process messages from userCtpComm_2 >----------------------

void userCtpCommExtractor_2() {
	while (true) {
		Message msg = userCtpComm_2.getMessage();
		ioMutex.lock();
		ioMutex.unlock();
		if (msg.command() == "FILE_REQUEST") {
			fileRequestBQ_2.enQ(msg);
		}
		else if (msg.command() == "FILE_COMPLETE") {
			fileMutex.unlock();
		}
		std::this_thread::sleep_for(
			std::chrono::milliseconds(contrivedExtractorDelayTime));
	}
}

//----< extract and process messages from fileRequestBQ_2 >-------------------

void fileRequestBQ_2Extractor() {
	while (true) {
		Message msg = fileRequestBQ_2.deQ();
		Message returnMsg(
			ctpEPforUserCtpCommunication_2, aUserEPforUserCtpCommunication_2);
		returnMsg.command("FILE_MESSAGE");
		returnMsg.name("5002");
		returnMsg.file(msg.attributes().at("file_requested"));
		fileMutex.lock();
		userCtpComm_2.postMessage(returnMsg);
	}
}

//----< extract and process messages from userCtpComm_3 >----------------------

void userCtpCommExtractor_3() {
	while (true) {
		Message msg = userCtpComm_3.getMessage();
		ioMutex.lock();
		ioMutex.unlock();
		if (msg.command() == "FILE_REQUEST") {
			fileRequestBQ_3.enQ(msg);
		}
		else if (msg.command() == "FILE_COMPLETE") {
			fileMutex.unlock();
		}
		std::this_thread::sleep_for(
			std::chrono::milliseconds(contrivedExtractorDelayTime));
	}
}

//----< extract and process messages from fileRequestBQ_3 >-------------------

void fileRequestBQ_3Extractor() {
	while (true) {
		Message msg = fileRequestBQ_3.deQ();
		Message returnMsg(
			ctpEPforUserCtpCommunication_3, aUserEPforUserCtpCommunication_3);
		returnMsg.command("FILE_MESSAGE");
		returnMsg.name("5003");
		returnMsg.file(msg.attributes().at("file_requested"));
		fileMutex.lock();
		userCtpComm_3.postMessage(returnMsg);
	}
}

//----< sends 100 file requests to server in quick succession >-----------------

void sendTestRequestMessagesToServer() {
	Message testRequestMsg(
		serverEPforUserServerCommunication, aUserEPforUserServerCommunication);
	testRequestMsg.command("TEST_REQUEST");
	testRequestMsg.name("5000");
	RequestSet reqSet;
	std::vector<std::string> testRequestVec = reqSet.getVector();
	int metaLoopNum = 10;
	// Send to server all test requests in testRequestVec metaLoopNum times
	for (int j = 0; j < metaLoopNum; j++) {
		for (int i = 0; i < testRequestVec.size(); i++) { // Send Test Requests
			testRequestMsg.attribute("BODY", reqSet[i]);
			userServerComm.postMessage(testRequestMsg);
		}
	}
}

//----< sends 10 * n file requests to server in quick succession >--------------

void sendTenTimesNTestRequestMessagesToServer(int n) {
	Message testRequestMsg(
		serverEPforUserServerCommunication, aUserEPforUserServerCommunication);
	testRequestMsg.command("TEST_REQUEST");
	testRequestMsg.name("5000");
	RequestSet reqSet;
	std::vector<std::string> testRequestVec = reqSet.getVector();
	int metaLoopNum = n;
	// Send to server all test requests in testRequestVec metaLoopNum times
	for (int j = 0; j < metaLoopNum; j++) {
		for (int i = 0; i < testRequestVec.size(); i++) { // Send Test Requests
			testRequestMsg.attribute("BODY", reqSet[i]);
			userServerComm.postMessage(testRequestMsg);
		}
	}
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
  std::string fileSpec =  sendFilePath + "/" + msg.file();
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
  fileMsg.file("MathTestDriver.dll");
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
  fileMsg.file("demoFile.txt");
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
