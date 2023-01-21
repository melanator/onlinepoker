/* 
	Based on
	Beej's Guide to Netwowk Programming
	https://beej.us/guide/bgnet/html/
*/
#include <iostream>
#include <string>
#include <vector>
#include "../../poker/include/poker.h"
#ifdef WIN_BUILD
#include <WS2tcpip.h>
#else
#define SOCKET int // In UNIX socket is just an int
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#endif

struct fork_struct{
	sockaddr_in client;
	SOCKET clientSocket;
};


int socket_handlinga(SOCKET clientSocket, sockaddr_in* client);
