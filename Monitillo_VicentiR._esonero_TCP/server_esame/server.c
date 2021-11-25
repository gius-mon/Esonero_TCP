/*
 * server.c
 *
 *  Created on: 11 nov 2021
 *      Author: giuse
 */
#include "service.h"



int main() {

	char operator;
	int first_operand = 0;
	int second_operand = 0;
	char sendString[BUFFERSIZE] = { };
	float op_result = 0;

	int port = PROTOPORT_SERVER;

#if defined WIN32 // initialize Winsock
	WSADATA wsa_data;
	int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (result != 0) {
		errorhandler("Error at WSAStartup()\n");
		return 0;
	}
#endif

	// SOCKET CREATION
	int my_socket;
	my_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (my_socket < 0) {
		errorhandler("socket creation failed.\n");
		clearwinsock();
		return -1;
	}
	// ASSIGNING A SOCKET ADDRESS
	struct sockaddr_in sad;
	memset(&sad, 0, sizeof(sad)); // ensures that extra bytes contain 0
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr(DEFAULT_SERVER_IP);
	sad.sin_port = htons(port); /* converts values between the host and
	 network byte order. Specifically, htons() converts 16-bit quantities
	 from host byte order to network byte order. */

	if (bind(my_socket, (struct sockaddr*) &sad, sizeof(sad)) < 0) {
		errorhandler("bind() failed.\n");
		closesocket(my_socket);
		clearwinsock();
		return -1;
	}

	// SETTING OF LISTEN SOCKET
	if (listen(my_socket, QLEN) < 0) {
		errorhandler("listen() failed.\n");
		closesocket(my_socket);
		clearwinsock();
		return -1;
	}

	printf("Server started... \n\n");
	fflush(stdout);
	// NEW CONNECTION ACCEPT
	struct sockaddr_in cad; // structure for the client address
	int client_socket; // socket descriptor for the client
	int client_len; // the size of the client address

	while (1) {

		printf("Waiting for a client to connect... \n");
		fflush(stdout);

		client_len = sizeof(cad); // set the size of the client address

		if ((client_socket = accept(my_socket, (struct sockaddr*) &cad,
				&client_len)) < 0) {
			errorhandler("accept() failed.\n");

			// CLOSE OF CONNECTION
			closesocket(client_socket);
			clearwinsock();
			return 0;
		}

		printf("\n Connection established with %s:%d\n", inet_ntoa(cad.sin_addr), port);
		fflush(stdout);

		int bytesRcvd;
		int totalBytesRcvd = 0;
		char buf[BUFFERSIZE] = { }; //buffer for data from the server

		while ((bytesRcvd = recv(client_socket, buf, BUFFERSIZE, 0)) > 0) { //this loop continues until the client close connection and recv fail

			totalBytesRcvd += bytesRcvd;

			printf("  Received: ");
			fflush(stdout); // Setup to print the echoed string

			printf("%s \n\n", buf);
			fflush(stdout);

			operator = 0;
			first_operand = 0;
			second_operand = 0;

			int risp = parser(buf, &operator, &first_operand, &second_operand); //allows to check if the input is correct and splits the string into the various parts

			buf[0] = '\0';

			if (risp == 0) {

				int status = 0;

				status = operation(operator, first_operand, second_operand, //do the operation between the two operand, and the result allows to check if the operation is possible
						&op_result);

				if (status == 0) {

					if (isInteger(op_result)) { // IF THE RESULT IS AN INTEGER DO CASTING
						itoa(op_result, sendString, 10);

					} else

					{
						gcvt(op_result, 10, sendString);  //convert floats number in string
					}


				}

				if (status == -1) {

					strcpy(sendString, "**Math error** \n");

				}

			} else {
				strcpy(sendString, "**Input not valid** \n");
			}

			int size = strlen(sendString);

			fflush(stdin);

			if (send(client_socket, sendString, size, 0) != size) {  // send the result of the operations as string

				errorhandler(
						"send() sent a different number of bytes than expected \n");
				closesocket(client_socket);
				clearwinsock();
				return -1;
			}

		}

		errorhandler(" recv() failed or connection closed \n\n");

	} // end-while

} // end-main

