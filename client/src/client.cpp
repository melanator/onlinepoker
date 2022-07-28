#include "../include/client.h"
using namespace std;

int main() {
	string ipAddress = "127.0.0.1";
	int port = 54000;
#ifdef WIN_BUILD
	// Initialize winsock
	WSAData wsaData;
	WORD ver = MAKEWORD(2,2);
	int wsOk = WSAStartup(ver, &wsaData);
	if (wsOk != 0){
		cout << "Cant initialize winsock!" << endl;
		return 1;
	}
#endif // WIN_BUILD

	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		cout << "Cant create a socket" << endl;
		return 2;
	}

	// Fiil in a hint sructure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	// Connet to a server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR){
		cout << "Cant connet to server" << endl;
		return 3;
	}

	// Dowhile loop to send and recieve data
	char buf[4096];
	string userInput;

	do{
		cout << "> ";
		getline(cin, userInput);

		// Send the text
		if (userInput.size() > 0){
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR){
				memset(buf, 0, 4096);
				// Wait for response
				int bytesReceived = recv(sock, buf, 4096, 0);
				// Echo response to console
				if (bytesReceived > 0)
				{
					cout << "SERVER> " << string(buf, 0, bytesReceived);
					cout << endl;
				}
			}
		}

	} while (userInput.size() > 0);

#ifdef WIN_BUILD
	//Close down
	closesocket(sock);
	WSACleanup();
#endif

	return 0;
}