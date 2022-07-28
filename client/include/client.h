/* 
	Based on
	Beej's Guide to Netwowk Programming
	https://beej.us/guide/bgnet/html/
*/
#include <iostream>
#include <string>
#ifdef WIN_BUILD
#include <WS2tcpip.h>
#else
#define SOCKET int // In UNIX socket is just an int
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#endif
