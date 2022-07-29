/* 
	Based on
	Beej's Guide to Netwowk Programming
	https://beej.us/guide/bgnet/html/
*/
#include "../include/server.h"

using namespace std;
DWORD WINAPI ThreadFun(LPVOID lpParam){
	cout << "Thread Running" << endl;
	return 0;
}

struct fork_struct{
	sockaddr_in client;
	SOCKET clientSocket;
};

int socket_handling(SOCKET clientSocket, sockaddr_in *client){	
	char host[NI_MAXHOST]; 		// Cleint's remote name
	char service[NI_MAXHOST];	// Port the client is connect on

	memset(host, 0, NI_MAXHOST);
	memset(service, 0, NI_MAXHOST);

	if(getnameinfo((sockaddr*)client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0){
		cout << host << " connected on port " << service << endl;
	} else {
		inet_ntop(AF_INET, &(*client).sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port " << ntohs((*client).sin_port) << endl;
	}

	char buf[4096];
	while (true){
		memset(buf, 0, sizeof(buf));

		// Wait for client to send data
		int bytesRecieved = recv(clientSocket, buf, size(buf), 0);
		if (bytesRecieved == SOCKET_ERROR) {
			cout << "Error in revc()" << endl;
			break;
		}
		if (bytesRecieved == 0){
			cout << "Client  " << host << ":" << ntohs((*client).sin_port) <<  " disconneted" << endl;
			break;
		}

		// Echo message back to client
		cout << "From " << host << " ";
		for(int i = 0; i < sizeof(buf); i++){
			cout << buf[i];
		}
		cout << endl;
		send(clientSocket, buf, bytesRecieved + 1, 0);
	}

	return 0;
}

DWORD windows_fork(LPVOID lpParam){
	fork_struct *params = static_cast<fork_struct*>(lpParam);

	sockaddr_in *client = &(*params).client;
	int clientSize = sizeof(client);
	SOCKET clientSocket = (*params).clientSocket;

	socket_handling(clientSocket, client);
	
	// Close the socket
	closesocket(clientSocket);
	return 0;
}

void linux_fork(sockaddr_in* client, SOCKET clientSocket){

	socket_handling(clientSocket, client);

	#ifndef WIN_BUILD
	close(clientSocket);
	#endif
}

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

	listen(listening, 2048);
	// Wait for a connection
	sockaddr_in client;
	int clientSize = sizeof(client);

	#ifdef WIN_BUILD
	HANDLE socketsThread[2048];
	fork_struct structsThread[2048];
	int current_socket = 0;

	while (true)
	{
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

		(structsThread[current_socket]).client = client;
		(structsThread[current_socket]).clientSocket = clientSocket;

		socketsThread[current_socket]  = CreateThread(
				NULL,
				0,
				windows_fork,
				(LPVOID) &(structsThread[current_socket]),
				0,
				NULL
			);

	}
	
	WSACleanup();
	#else
		fork()
	#endif // WIN_BUILD

	return 0;
}