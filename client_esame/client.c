/*
 * client.c
 *
 *  Created on: 11 nov 2021
 *      Author: giuse
 */

#include "comunication_protocol.h"
#include <stdio.h>
#include <winsock.h>
#include "service.h"


#define BUFFERSIZE 512



int main(void) {


	char ip_address[] = DEFAULT_CLIENT_IP;
	int port_number = PROTOPORT_CLIENT;

	set_ip_port( &ip_address, &port_number);





#if defined WIN32
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("error at WSASturtup\n");
		system("PAUSE");

		fflush(stdout);
		return -1;
	}
#endif

	// SOCKET CREATION
	int Csocket;
	Csocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (Csocket < 0) {
		ErrorHandler("socket creation failed.\n");
		system("PAUSE");

		closesocket(Csocket);
		ClearWinSock();
		return -1;
	}

	// BUILD SERVER ADRESS
	struct sockaddr_in sad;
	memset(&sad, 0, sizeof(sad));
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr(ip_address); // server IP
	sad.sin_port = htons(port_number); // Server port

	// SERVER CONNESSION
	if (connect(Csocket, (struct sockaddr*) &sad, sizeof(sad)) < 0) {
		ErrorHandler("Failed to connect.\n");
		system("PAUSE");

		closesocket(Csocket);
		ClearWinSock();
		return -1;
	}

	while (1 == 1) {

		printf(
				"\n\n Please, enter the operation to execute as follows \n Ex. + 34 7 \n operation entered: ");
		fflush(stdout);

		char operation[BUFFERSIZE] = { };

		printf("%s ", operation);
		fflush(stdout);	//
		fflush(stdin);

		gets(operation);

		int size = sizeof(operation);

		if (operation[0] == '=') {
			closeConnection(Csocket);
			printf(" Press a key to continue...");
			getch();

			return (0);

		}

		fflush(stdout);
		// SEND DATA TO THE SERVER
		if (send(Csocket, operation, size, 0) != size) {

			ErrorHandler(
					"send() sent a different number of bytes than expected \n");
			printf("Press a key to continue...");
						getch();

			closesocket(Csocket);
			ClearWinSock();
			return -1;
		}

		// RECEIVE DATA FROM THE SERVER
		int bytesRcvd = 0;
		char buf[BUFFERSIZE] = { }; // buffer for data from the server
		printf(" Result: ");
		fflush(stdout); // Setup to print the echoed string

		if ((bytesRcvd = recv(Csocket, buf, BUFFERSIZE - 1, 0)) <= 0) {
			ErrorHandler("recv() failed or connection closed prematurely \n");
			printf(" \nPress a key to continue...");
									getch();

			closesocket(Csocket);
			ClearWinSock();
			return -1;
		}

		printf("%s \n", buf);
		fflush(stdout); // Print the echo buffer
		printf(" Press a key to continue...");
								getch();
	}

}

