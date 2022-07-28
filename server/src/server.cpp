/* 
	Based on
	Beej's Guide to Netwowk Programming
	https://beej.us/guide/bgnet/html/
*/
#include "../include/server.h"

using namespace std;

int main() {
	#ifdef WIN_BUILD
	// Initialize winsock
	WSADATA wsaData;  
	WORD ver = MAKEWORD(2,2); // Version
	
	int wsOk = WSAStartup(ver, &wsaData);
	if (wsOk != 0){
		cout << "Cant initialize winsock!" << endl;
		return 1;
	}

	#else
	// Initialize sock on Linux
	#endif // WIN_BUILD

	// Create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET) {
		cout << "Cant create a socket" << endl;
		return 2;
	}

	// Bind socket ip address and port to a socket 
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.s_addr = INADDR_ANY;

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	listen(listening, SOMAXCONN);
	// Wait for a connection
	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(
		listening, 
		(sockaddr*)&client, 
			#ifndef WIN_BIULD // Unix build size signature in socklen_t*, windows int*
			(socklen_t*)
			#endif	
		&clientSize);
	if (clientSocket == INVALID_SOCKET){
		cout << "Failed client Socket";
		return 3;
	}

	char host[NI_MAXHOST]; 		// Cleint's remote name
	char service[NI_MAXHOST];	// Port the client is connect on

	memset(host, 0, NI_MAXHOST);
	memset(service, 0, NI_MAXHOST);

	if(getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0){
		cout << host << " connnnected on port " << service << endl;
	} else {
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port " << ntohs(client.sin_port) << endl;
	}

	// Close listening socket
#ifdef WIN_BUILD
	closesocket(listening);
#else
	close(listening);
#endif // WIN_BUILD


	// While loop: accept and echo back message to client
	char buf[4096];

	while (true){
		memset(buf, 0, 4096);

		// Wait for client to send data
		int bytesRecieved = recv(clientSocket, buf, 4096, 0);
		if (bytesRecieved == SOCKET_ERROR) {
			cout << "Error in revc()" << endl;
			break;
		}
		if (bytesRecieved == 0){
			cout << "Client disconneted" << endl;
			break;
		}

		// Echo message back to client
		cout << "From " << host << " ";
		for(int i = 0; i < 4096; i++){
			cout << buf[i];
		}
		cout << endl;
		send(clientSocket, buf, bytesRecieved + 1, 0);
	}
	
	// Close the socket
#ifdef WIN_BUILD
	closesocket(clientSocket);
	// Clean WSocket
	WSACleanup();
#else
	close(clientSocket);
#endif // WIN_BUILD


	return 0;
}