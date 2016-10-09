/*
 * client.c
 *
 *  Created on: 2016年10月5日
 *      Author: darmenliu
 */
#include <stdio.h>
#include "socket-api.h"

int main()
{
	char *server = "10.255.20.160";
        int port = 44324;
        int err = 0;
        int server_sock, client_sock;

	err = tcp_stream_client(server, port, 1, &server_sock);
	if(err < 0)
	{
		printf("open tcp server socket failed\n");
		return 0;
	}
	else
	{
		printf("connect the server successfully\n");
	}
	
	return 0;
}




