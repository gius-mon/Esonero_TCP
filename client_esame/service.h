/*
 * service.h
 *
 *  Created on: 23 nov 2021
 *      Author: giuse
 */

#ifndef SERVICE_H_
#define SERVICE_H_


void set_ip_port(char* ip_address, int* port_number);

void ErrorHandler(char *errorMessage) ;

void ClearWinSock();

int closeConnection(int socket);


#endif /* SERVICE_H_ */
