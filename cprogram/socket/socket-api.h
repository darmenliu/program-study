/*
 * socket-api.h
 *
 *  Created on: 2016年10月5日
 *      Author: darmenliu
 */

#ifndef CPROGRAM_SOCKET_SOCKET_API_H_
#define CPROGRAM_SOCKET_SOCKET_API_H_

int tcp_stream_server(char *host_server, int port, int non_block, int *fd);
int tcp_stream_client(char *host_server, int port, int non_block, int *fd);




#endif /* CPROGRAM_SOCKET_SOCKET_API_H_ */
