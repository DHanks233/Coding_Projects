//-----------------------------------------------------------------------------
// File:          UdpRelay.cpp
// Classes:       UdpRelay
//
// Class Methods Implemented:
//                UdpRelay(const char* ipPlusPort);
//                ~UdpRelay();
//                void sendLocalMessage(char * currentMessage);
//                void recvLocalMessage(char * currentMessage);
//                static void* commandThread(void* arg);
//                static void* acceptThread(void* arg);
//                static void* relayInThread(void* arg);
//                static void* relayOutThread(void *arg);
//                bool isDuplicatePacket(char* currentPacket);
//                void putIPIntoPacket(char* currentPacket);
//                char* getIPNumber();
//                char* getOutThreadCnx(int sd);
//                void addExpiredOutThread(int sd);
//                UdpRelay();
//                void addRemoteIP(string remoteGroupID);
//                void terminateRemoteCxn(string remoteGroupID);
//                void deleteRemoteIP();
//                void showTCPConnections();
//                void displayHelpMenu();
//                void terminateUpdRelay();
//                void setIpChars();
//                void tcpMultiCastToRemoteGroups(char* outPacket);
//                void createUdpRelayServerConnection();
//                void terminateAllTcpConnections();
//
// Written By:    Tyler Laws and Daniel Hanks
// Last Modified: June 5, 2015
// Contents: UdpRelay class definitions
//-----------------------------------------------------------------------------
#include "UdpRelay.h"

//-----------------------------------------------------------------------------
// UdpRelay Constructor
// Parses command line input into IP and port numbers, instantiates all data
// members, spins up command, relayIn and accept threads then calls a semaphore
// wait until a "quit" command is issued.
//
// @pre:   char* parameter is a valid IP number concatenated with a port number
// @post:  3 threads are spun up, IP and port numbers are saved
// @param *ipPlusPort:  The IP address and port number: (XXX.XXX.XXX.XXX:YYYYY)
// @throw: throws invalid_argument if ipPlusPort is not the correct length
//-----------------------------------------------------------------------------
UdpRelay::UdpRelay(const char* ipPlusPort) {

  if (strlen(ipPlusPort) != ARGUMENT_SIZE) {
    throw invalid_argument("Incorrect IP or port numbers.");
  }

  ipNumber = new char[16];
  char portNum[6];
  memcpy(ipNumber, ipPlusPort, IP_SIZE);
  memcpy(portNum, &ipPlusPort[16], PORT_SIZE);
  portNumber = atoi(portNum);
  relaySock = new Socket(portNumber);
  cout << "UdpRelay: booted up at " << ipNumber << ":" << portNumber << endl;
  setIpChars();
  sem_init(&mutex, 0, 0);

  pthread_t commandThreadID;
  pthread_create(&commandThreadID, NULL, commandThread, (void*)this);
  pthread_t inRelayThreadID;
  pthread_create(&inRelayThreadID, NULL, relayInThread, (void*)this);
  pthread_t acceptRelayThreadID;
  pthread_create(&acceptRelayThreadID, NULL, acceptThread, (void*)this);

  sem_wait(&mutex);
  pthread_cancel(commandThreadID);
  pthread_cancel(inRelayThreadID);
  pthread_cancel(acceptRelayThreadID);
}

//-----------------------------------------------------------------------------
// UdpRelay Destructor
// Deletes any dynamically allocated data members
//
// @pre:   None
// @post:  char * ipNumber is deleted
//-----------------------------------------------------------------------------
UdpRelay::~UdpRelay() {
  if (ipNumber != NULL) {
    delete[] ipNumber;
    ipNumber = NULL;
  }
  if(relaySock != NULL) {
    delete relaySock;
    relaySock = NULL;
  }
}

//-----------------------------------------------------------------------------
// sendLocalMessage
// Broadcasts the char* parameter via UDP
//
// @pre:   currentMessage is not NULL
// @post:  currentMessage is broadcast via UDP
// @param  *currentMessage: The message to broadcast UDP
//-----------------------------------------------------------------------------
void UdpRelay::sendLocalMessage(char * currentMessage) {
  UdpMulticast udp(ipNumber, portNumber);
  if(udp.getClientSocket() == NULL_SD) {
    cout << "UdpMulticast client socket could not be obtained." << endl;
    return;
  }
  udp.multicast(currentMessage);
}

//-----------------------------------------------------------------------------
// recvLocalMessage
// Receives a local UDP broadcast
//
// @pre:   currentMessage is not NULL and is of at least SIZE length
// @post:  The message received via UDP is copied into currentMessage
// @param  *currentMessage: The buffer that will contain the message received
//-----------------------------------------------------------------------------
void UdpRelay::recvLocalMessage(char * currentMessage) {
  UdpMulticast udp(ipNumber, portNumber);
  if(udp.getServerSocket() == NULL_SD) {
    cout << "UdpMulticast server socket could not be obtained." << endl;
    return;
  }
  while (true) {
    if (udp.recv(currentMessage, SIZE)) {
      break;
    }
  }
}

//-----------------------------------------------------------------------------
// commandThread
// A static class method that is a thread function for the command thread
//
// @pre:   *arg parameter represents a valid UdpRelay object
// @post:  Acts based on user input, eventually terminating upon quit command
// @param  *arg:  A void pointer to the UdpRelay object creating the thread
//-----------------------------------------------------------------------------
void* UdpRelay::commandThread(void* arg) {
  UdpRelay* sourceUdpRelay = (UdpRelay*)arg;
  string currCommand = "";
  string commandParam = "";
  while (true) {
    cout << "% ";
    cin >> currCommand;
    if (currCommand.compare(0, 3, "add") == 0) {
      cin >> commandParam;
      sourceUdpRelay->addRemoteIP(commandParam);
    } else if (currCommand == "delete") {
      cin >> commandParam;
      sourceUdpRelay->terminateRemoteCxn(commandParam);
    } else if (currCommand == "show") {
      sourceUdpRelay->showTCPConnections();
    } else if (currCommand == "help") {
      sourceUdpRelay->displayHelpMenu();
    } else if (currCommand == "quit") {
      sourceUdpRelay->terminateAllTcpConnections();
      sem_post(&sourceUdpRelay->mutex);
      break;
    } else {
      cout << "Command not recognized." << endl;
      sourceUdpRelay->displayHelpMenu();
      continue;
    }
  }
}

//-----------------------------------------------------------------------------
// relayInThread
// A static class method that is a thread function for the relayIn thread. It
// loops continually, receiving UDP messages and sending them via TCP to nodes
// connected to this UdpRelay object
//
// @pre:   *arg parameter represents a valid UdpRelay object
// @post:  None
// @param  *arg:  A void pointer to the UdpRelay object creating the thread
//-----------------------------------------------------------------------------
void* UdpRelay::relayInThread(void* arg) {
    UdpRelay * currInRelay = (UdpRelay*)arg;
    char inPacket[SIZE] = {0};
    while(true) {
      currInRelay->recvLocalMessage(inPacket);
      if(!currInRelay->isDuplicatePacket(inPacket)) {
        currInRelay->putIPIntoPacket(inPacket);
        currInRelay->tcpMultiCastToRemoteGroups(inPacket);
      }
      memset(inPacket, 0, SIZE);
    }
}

//-----------------------------------------------------------------------------
// addRemoteIp
// Takes a group IP/name and port number parameter and opens a TCP connection
// to that node. Sends the hostname of this machine to the remote node and
// updates the tcpCxns map
//
// @pre:   remoteGroupID parameter is a valid group IP and port number
// @post:  Socket is opened for TCP and tcpCxns map is updated
// @param  remoteGroupID: An group IP/name and port (XXX.XXX.XXX.XXX:YYYYY)
//-----------------------------------------------------------------------------
void UdpRelay::addRemoteIP(string remoteGroupID) {
  const char DELIMITER = ':';
  const int DELIM_POS = remoteGroupID.find(DELIMITER);
  const int LENGTH = remoteGroupID.length();
  string remoteIPAddress = remoteGroupID.substr(0, DELIM_POS);
  string remotePort = remoteGroupID.substr(DELIM_POS + 1, LENGTH);
  int sd = 0;
  char* clientSock = new char[GROUP_LENGTH];
  memset(clientSock, 0, GROUP_LENGTH);
  strcpy(clientSock, remoteIPAddress.c_str());
  sd = relaySock->getClientSocket(clientSock);
  cout << "Registered: " << clientSock << endl;
  if(sd == -1) {
    cerr << "upd relay error establishing remote tcp connection" << endl;
  }
  else {
    checkForDuplicateCxn(remoteGroupID);
    if(outThreads.count(sd) > 0) {
      if(expiredOutThreads.size() > 5) {
        while(!expiredOutThreads.empty()) {
          pthread_join(expiredOutThreads.front(), NULL);
          expiredOutThreads.pop();
        }
      }
      outThreads.erase(sd);
    }
    outThreads.insert(pair<int,pthread_t>(sd, sd));
    tcpCxns[clientSock] = sd;
    cout << "Added: " << clientSock << ":" << sd << endl;
    char hostName[GROUP_LENGTH] = {0};
    gethostname(hostName, GROUP_LENGTH);
    send(sd, hostName, GROUP_LENGTH, 0);
    outThreadInfo * tempStruct = new outThreadInfo;
    tempStruct->remoteHostName = remoteIPAddress;
    tempStruct->currentRelay = this;
    tempStruct->socketNumber = sd;
    pthread_create(&outThreads[sd], NULL, relayOutThread, (void*)tempStruct);
  }
  if(clientSock != NULL) {
    delete clientSock;
    clientSock = NULL;
  }
}

//-----------------------------------------------------------------------------
// checkForDuplicateCxn
// Checks if the remote connection already exists and if it does, it deletes
// the already existing connection
//
// @pre:   string shall be a valid remote group id host name
// @post:  if a duplicate connection already existed, the previous connection
//         will be closed and deleted from the connections map. Otherwise,
//         there are no changes.
// @param  const string& GRP_ID: remote host name
//-----------------------------------------------------------------------------
void UdpRelay::checkForDuplicateCxn(const string& GRP_ID) {
  if(tcpCxns.count(GRP_ID) > 0) {
    close(tcpCxns[GRP_ID]);
    tcpCxns.erase(tcpCxns.find(GRP_ID));
  }
}

//-----------------------------------------------------------------------------
// displayHelpMenu
// Called by commandThread to send all available user commands to cout
//
// @pre:   None
// @post:  None
//-----------------------------------------------------------------------------
void UdpRelay::displayHelpMenu() {
  cout << "UdpRelay.commandThread: accepts..." << endl;
  cout << "\tadd remoteIP:remoteTcpPort | Adds TCP connection to remoteIP"
       << endl;
  cout << "\tdelete remoteIP | Remove TCP connection at remoteIP" << endl;
  cout << "\tshow | Show current TCP connections" << endl;
  cout << "\thelp | Display all commands" << endl;
  cout << "\tquit | Terminate the UdpRelay program" << endl;
}

//-----------------------------------------------------------------------------
// acceptThread
// A static class method that is a thread function for the accept thread, which
// loops continually accepting TCP connection requests
//
// @pre:   *arg parameter represents a valid UdpRelay object
// @post:  Opens a TCP connection socket and updates a connection map
// @param  *arg:  A void pointer to the UdpRelay object creating the thread
//-----------------------------------------------------------------------------
void * UdpRelay::acceptThread(void* arg) {
  UdpRelay * sourceRelay = (UdpRelay*)arg;
  sourceRelay->createUdpRelayServerConnection();
}

//-----------------------------------------------------------------------------
// isDuplicatePacket
// Checks the header of the packet and returns true if the local group IP is
// already contained in the header (a duplicate message), false otherwise
//
// @pre:   currentPacket parameter is of the correct packet format
// @post:  None
// @param  currentPacket: The packet received via UDP or TCP
// @returns bool:         True if the current UdpRelay's IP is contained in
//                        the packet header, false otherwise
//-----------------------------------------------------------------------------
bool UdpRelay::isDuplicatePacket(char* currentPacket) {
  int hop = currentPacket[3];
  int counter = 0;

  for (int i = 1; i <= hop; i++) {
    int ipReadIn = 0;
    counter = 0;
    for (int j = (4 * i); j < (4 * (i + 1)); j++) {
      if (currentPacket[j] == ipChars[counter]) {
        ipReadIn++;
      }
      counter++;
    }
    if (ipReadIn == 4) {
      return true;
    }
  }
  return false;
}

//-----------------------------------------------------------------------------
// setIpChars
// Takes the group IP number of format (2XX.255.255.255) and puts each set of
// three numbers into a single char value before storing them in ipChars[]
//
// @pre:   ipNumber is of valid format and ipChars is instantiated
// @post:  ipChars[] array contains the 4-char version of the group IP
//-----------------------------------------------------------------------------
void UdpRelay::setIpChars() {
  stringstream ss;
  int currentNumber = 0;
  for (int i = 0; i < 4; i++) {
    ss << ipNumber + (i * 4);
    ss >> currentNumber;
    ipChars[i] = currentNumber;
    ss.clear();
    ss.str(string());
  }
}

//-----------------------------------------------------------------------------
// putIPIntoPacket
// Takes a char* packet passed as parameter and adds group IP address of the
// current UdpRelay node into the header in the format of a single byte for
// each 3-digit portion of the IP, then increments the "hop" number
//
// @pre:   currentPacket has valid packet format
// @post:  None
// @param  currentPacket: A packet in valid format described in UdpRelay header
//-----------------------------------------------------------------------------
void UdpRelay::putIPIntoPacket(char* currentPacket) {
  int offset = 4 + (currentPacket[3] * 4);
  char tempMessage[SIZE] = {0};
  memcpy(tempMessage, currentPacket + offset, SIZE - offset);
  memcpy(currentPacket + offset, ipChars, 4);
  memcpy(currentPacket + offset + 4, tempMessage, SIZE - offset - 4);
  currentPacket[3] += 1;
}

//-----------------------------------------------------------------------------
// relayOutThread
// A static class method that is a thread function for the relayOut thread,
// which spins up when a TCP connection is established. Receives messages via
// TCP connection and broadcasts them using UDP, if not a duplicate message
//
// @pre:   *arg parameter represents a valid outThreadInfo struct
// @post:  Removes the connection from the map when the thread terminates
// @param  *arg:  Pointer to the outThreadInfo struct with socket and UdpRelay
//-----------------------------------------------------------------------------
void* UdpRelay::relayOutThread(void *arg) {
  outThreadInfo *outInfo = (outThreadInfo*)arg;
  UdpRelay* thisUdpRelay = outInfo->currentRelay;
  int sd = outInfo->socketNumber;
  string remoteName = outInfo->remoteHostName;
  delete (outThreadInfo*)arg;
  char outPacket[SIZE] = {0};
  char outMsg[SIZE] = {0};
  while(true) {
    if(recv(sd, outPacket, SIZE, 0) <= 0) {
      break;
    }
    if(!thisUdpRelay->isDuplicatePacket(outPacket)) {
      int hop = outPacket[3];
      int offset = 4 + (hop * 4);
      memcpy(outMsg, outPacket + offset, SIZE - offset);
      cout << "UdpRelay: received " << strlen(outPacket) << " bytes from "
          << remoteName << " = " << outMsg << endl;
      thisUdpRelay->putIPIntoPacket(outPacket);
      thisUdpRelay->sendLocalMessage(outPacket);
      cout << "UdpRelay: broadcast buf[" << strlen(outPacket) << "] to "
          << thisUdpRelay->getIPNumber() << ":" << PORT_NUM << endl;
    }
  }
  if(thisUdpRelay->tcpCxns.count(remoteName) > 0) {
    thisUdpRelay->tcpCxns.erase(thisUdpRelay->tcpCxns.find(remoteName));
  }
  thisUdpRelay->addExpiredOutThread(sd);
}

//-----------------------------------------------------------------------------
// tcpMulticastToRemoteGroups
// Sends a message via TCP to all remote nodes connected to this UdpRelay node,
// and informs the user what message was sent and how many bytes
//
// @pre:   outPacket has valid packet format
// @post:  None
// @param  outPacket: A packet received via UDP to be sent out via TCP
//-----------------------------------------------------------------------------
void UdpRelay::tcpMultiCastToRemoteGroups(char* outPacket) {
  char outMsg[SIZE];
  int hop = outPacket[3];
  int offset = 4 + (hop * 4);
  memcpy(outMsg, outPacket + offset, SIZE - offset);

  for(map<string, int>::iterator curSdIt = tcpCxns.begin();
      curSdIt != tcpCxns.end(); curSdIt++) {
    if(send(curSdIt->second, outPacket, SIZE, 0) <= 0) {
      close(curSdIt->second);
      tcpCxns.erase(curSdIt->first);
    }
    cout << "UdpRelay: relay " << outMsg << " to remoteGroup["
        << curSdIt->first << "]" << endl;
  }
}

//-----------------------------------------------------------------------------
// terminateRemoteCxn
// Closes the socket to the remote node IP/name passed as parameter, then
// deletes that connection from the map
//
// @pre:   remoteGroupID is a valid group IP/name and map contains that group
// @post:  tcpCxns map is updated with group IP/name entry removed
// @param  remoteGroupID: A valid group IP/Name
//-----------------------------------------------------------------------------
void UdpRelay::terminateRemoteCxn(string remoteGroupID) {
  char* remoteSocket = new char[remoteGroupID.length() + 1];
 strcpy(remoteSocket, remoteGroupID.c_str());
  if(tcpCxns.count(remoteSocket) != 0) {
    shutdown(tcpCxns[remoteSocket], SHUT_RDWR);
    if(expiredOutThreads.size() > 5) {
      while(!expiredOutThreads.empty()) {
        pthread_join(expiredOutThreads.front(), NULL);
        expiredOutThreads.pop();
      }
    }
    if(tcpCxns.count(remoteSocket) > 0) {
      tcpCxns.erase(tcpCxns.find(remoteSocket));
      cout << "UdpRelay: deleted " << remoteGroupID << endl;
    }
  }
  else {
    cout << "No connection to that remote group exists." << endl;
  }
}

//-----------------------------------------------------------------------------
// terminateAllTcpConnections
// Closes all open TCP sockets and removes the connection entries from both
// tcpCxns and outThreadCxns maps
//
// @pre:   None
// @post:  Sockets are closed and both maps have all entries deleted
//-----------------------------------------------------------------------------
void UdpRelay::terminateAllTcpConnections() {
  for(map<string, int>::iterator curSdIt = tcpCxns.begin();
      curSdIt != tcpCxns.end(); curSdIt++) {
    close(curSdIt->second);
    tcpCxns.erase(curSdIt->first);
  }
}

//-----------------------------------------------------------------------------
// getIPNumber
// Returns the group IP number used at command line execution
//
// @pre:   ipNumber is not NULL
// @post:  None
// @returns char*:  Group IP number
//-----------------------------------------------------------------------------
char* UdpRelay::getIPNumber() {
  return ipNumber;
}

//-----------------------------------------------------------------------------
// showTCPConnections
// Displays all open TCP connections, either outgoing or incoming, to cout. If
// no connections exist, tells user there are no open connections
//
// @pre:   None
// @post:  None
//-----------------------------------------------------------------------------
void UdpRelay::showTCPConnections() {
  if(tcpCxns.size()) {
    cout << "UdpRelay: TCP connections to remote groups:" << endl;
    for(map<string, int>::iterator curSdIt = tcpCxns.begin();
        curSdIt != tcpCxns.end(); curSdIt++) {
      cout << curSdIt->first << " on socket: " << curSdIt->second << endl;
    }
  }
  if(tcpCxns.empty()) {
    cout << "No TCP connections currently established." << endl;
  }
}

//-----------------------------------------------------------------------------
// addExpiredOutThread
// Pushes a socket descriptor that has been closed into the expiredOutThreads
// queue
//
// @pre:   sd is a closed socket descriptor
// @post:  sd is added to expiredOutThreads
// @param  sd: a socket descriptor
//-----------------------------------------------------------------------------
void UdpRelay::addExpiredOutThread(int sd) {
  expiredOutThreads.push(sd);
}

//-----------------------------------------------------------------------------
// createUdpRelayServerConnection
// Called by acceptThread, this method creates a TCP server socket and connects
// to a TCP client, receives their group name/IP and spins up a relayOut thread
// before adding the new connection the outThread and outThreadCxns maps
//
// @pre:   None
// @post:  outThread and outThreadCxns maps are updated, a socket is opened
//-----------------------------------------------------------------------------
void UdpRelay::createUdpRelayServerConnection() {
  int sd = 0;
  char ip[IP_SIZE + 1] = {0};
  while(true) {
    outThreadInfo * tempStruct = new outThreadInfo;
    tempStruct->currentRelay = this;
    sd = relaySock->getServerSocket();
    tempStruct->socketNumber = sd;
    recv(sd, ip, GROUP_LENGTH, 0);
    cout << "Registered: " << ip << endl;
    if(outThreads.count(sd) > 0) {
      if(expiredOutThreads.size() > 5) {
        while(!expiredOutThreads.empty()) {
          pthread_join(expiredOutThreads.front(), NULL);
          expiredOutThreads.pop();
        }
      }
      outThreads.erase(sd);
    }
    outThreads.insert(pair<int,pthread_t>(sd, sd));
    tcpCxns.insert(pair<string,int>(ip, sd));
    tempStruct->remoteHostName = ip;
    pthread_create(&outThreads[sd], NULL, relayOutThread, (void*)tempStruct);
  }
}
