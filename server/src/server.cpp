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

	fd_set master;
	FD_ZERO(&master);

	FD_SET(listening, &master);

	while (true) {
		fd_set copy = master;	
		char buf[4096];
		
		int max_fd = 0;
		int socketCount = select(max_fd + 1, &copy, nullptr, nullptr, nullptr);
		
		for (size_t i = 0; i < socketCount; i++)
		{
			SOCKET sock = copy.fd_array[i];
			if (sock == listening) {	
				char host[NI_MAXHOST]; 		// Cleint's remote name
				char service[NI_MAXHOST];	// Port the client is connect on

				memset(host, 0, NI_MAXHOST);
				memset(service, 0, NI_MAXHOST);

				sockaddr_in client;
				int clientSize = sizeof(client);

				// Accept a new connection
				SOCKET clientSocket = accept(listening, (sockaddr *)&client, &clientSize);
				if(getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0){
					cout << host << " connected on port " << service << endl;
				} else {
					inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
					cout << host << " connected on port " << ntohs(client.sin_port) << endl;
				}

				// Add the new connection to the list of cleints
				FD_SET(clientSocket, &master);
				max_fd++;

				// Broadcast we have a new connection
				string msg = "Hello";
				send(clientSocket, msg.c_str(), msg.size() + 1, 0);

			}
			else {
				memset(buf, 0, 4096);
				int bytesIn = recv(sock, buf, 4096, 0);

				if (bytesIn <= 0) {
					cout << "Disconnected" << endl;
					closesocket(sock);
					FD_CLR(sock, &master);
					max_fd--;
				} 
				else {
					cout << buf << endl;
					int sendResult = send(sock, buf, 4096, 0);
				}	
				// Send message to other cleint
			}
		}

	}

	#ifdef WIN_BUILD
	WSACleanup();
	#endif // WIN_BUILD

	return 0;
}