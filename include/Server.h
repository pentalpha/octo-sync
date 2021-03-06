#ifndef _SERVER_
#define _SERVER_

/*#include <iostream>     //cout
#include <cstring>      //memset
#include <cstdlib>      //exit
#include <netinet/in.h> //htons
#include <arpa/inet.h>  //inet_addr
#include <sys/socket.h> //socket
#include <unistd.h>     //close
#include <thread>
#include "StringQueue.h"
#include <string>*/
#include "Socket.h"

/*#define MAXMSG 1024

using namespace std;

class Server{
public:
  Server(const char* localIP, int portNumber);

  bool start();
  void startWaiting();
  bool isConnected();
  bool isWaiting();
  void stop();
  string getMessage();
private:
  void waitForClientAndReceive();
  bool getSocket();
  
  bool startListening();
  StringQueue messages;
  bool connected;
  
  bool exitFlag;
  //variáveis do servidor
  struct sockaddr_in address;
  int socketId;
  //variáveis relacionadas com as conexões clientes
  struct sockaddr_in addressClient;
  socklen_t sizeAddressClient;
  int connectionClientId;
  //Permitir inserir o caractere de fim de msg \0
  char *msg;
  int bytesread;
};*/

class Server : public Socket{
public:
  Server(const char* localIP, int portNumber);
  bool startTransaction();
  bool isWaiting();
  bool isBinded();
  bool sendMsg(std::string str);
  std::string clientAddress;
protected:
  int waitForClient();
  bool doBind();
  bool startListening();
  bool waitingFlag, bindedFlag;
  //client related information
  struct sockaddr_in addressClient;
  socklen_t sizeAddressClient;
  int connectionClientId;
};

#endif