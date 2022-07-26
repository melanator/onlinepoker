#include <iostream>

// On windows, there is another library for sockets
#ifdef WIN_BUILD
#include <ws2tcpip.h>
#else
#include <sys/sockets.h>
#endif

using namespace std;

int main() {
	#ifdef WIN_BUILD
	// Initialize winsock
	WSADATA wsaData;  
	WORD ver = MAKEWORD(2,2); // Version
	
	int wsOk = WSAStartup(ver, &wsaData);
	if (wsOk != 0){
		cout << "Cant initialize winsock!" << endl;
	}

	// Create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET) {
		cout << "Cant create a socket" << endl;
		return 2;
	}
	#else
	// Initialize sock on Linux

	#endif
	

	// Bind socket ip address and port to a socket 
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;


	cout << "Server" << endl;
	return 0;
}