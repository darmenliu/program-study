/*
 * server.c
 *
 *  Created on: 2016年10月5日
 *      Author: darmenliu
 */
#include <stdio.h>
#include "socket-api.h"

int main()
{
	char *localhost = "192.168.1.110";
	int port = 44324;
	int err = 0;
	int server_sock, client_sock;

	err = tcp_stream_server(localhost, port, 1, &server_sock);
	if (err < 0)
	{
		printf("open tcp server socket failed\n");
		return 0;
	}

	while (1)
	{
		err = tcp_stream_accept(server_sock, &client_sock);
		if (err < 0)
		{
			printf("tcp server accept failed\n");
			break;
		}

		tcp_socket_close(client_sock);
	}
	tcp_socket_close(server_sock);
	return 0;
}




