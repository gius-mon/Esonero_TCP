/*
 * service.c
 *
 *  Created on: 23 nov 2021
 *      Author: giuse
 */

#include <stdio.h>
#include <winsock.h>

void ErrorHandler(char *errorMessage) {
	printf("%s", errorMessage);
	fflush(stdout);
}

void ClearWinSock() {
#if defined WIN32
	WSACleanup();
#endif
}

int closeConnection(int socket) {

	closesocket(socket);
	ClearWinSock();
	printf("\n");
	fflush(stdout);

	return 0;

}

void set_ip_port(char* ip_address, int* port_number){

char choice;

	printf(" Welcome!\n\n"); //prints welcome message

	printf(" Do you want to enter the port number and ip address? Y/N \n");
			scanf("%c", &choice);
			fflush(stdout);

	while (choice != 'y' && choice != 'Y' && choice != 'N' && choice != 'n') {
		system("cls");

		fflush(stdin);

		printf("Error !Enter Y if you want to enter the port number and ip address, enter N otherwise \n");
		scanf("%c", &choice);
		fflush(stdout);}

		/*switch case of persons choice*/

		if ( choice == 'y'|| choice == 'Y') {
			printf("\n Enter IP adress -> ");
			fflush(stdin);
			int error =scanf("%s", ip_address);

			//check on the length of the ip
			while (error != 1 || strlen(ip_address) < 7 || strlen(ip_address) > 15) {

				printf("\nError! Invalid IP Address! \n");
				fflush(stdout);

				printf(" IP address: ");
				fflush(stdout);

				fflush(stdin);
				error =scanf("%s", ip_address);
			}

			printf("\n Enter the number port -> ");

			while (scanf("%d", port_number) != 1) {
				printf(
						"\nError! I expected a number not a string!\n Please, enter the correct one! \n");
				fflush(stdin);

				/* remove any characters left in the buffer up to newline */
				while (getchar() != '\n');

			}

		}

}
