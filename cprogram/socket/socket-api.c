#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "socket-api.h"

enum SA_ERROR {
    SOCKET_ERROR = -1,
    NON_BLOCK_ERROR,
	GET_ADDR_ERROR,
	BIND_ADDR_ERROR,
	LISTEN_ERROR,
	CONNECT_ERROR,
	ACCEPT_ERROR,
};

#define LENGTH_OF_LISTEN_QUEUE 64

int set_nonblocking(int fd, int nonblocking)
{
	int flag = fcntl(fd, F_GETFD);
	int err = 0;

	if (flag < 0)
	{
	    return NON_BLOCK_ERROR;
	}

	if (nonblocking)
	{
		err = fcntl(fd, F_SETFD, flag | O_NONBLOCK);
	}
	else
	{
		err = fcntl(fd, F_SETFD, flag | ~O_NONBLOCK);
	}
	if (err < 0)
	{
		return NON_BLOCK_ERROR;
	}
    return 0;
}

int get_socket_addr(char *host_server, int port, int type, struct sockaddr_in *addr)
{
    struct addrinfo *addrlist = 0;
    struct addrinfo hint;
    
    char ports[20];
    int err = 0;
    int family = AF_INET;
    int protocol = 0;
    
    if (host_server)
    {
       memset(&hint, 0, sizeof(hint)); 
       hint.ai_family = family;
       hint.ai_socktype = type;
       hint.ai_protocol = protocol;
       hint.ai_canonname = NULL;
       hint.ai_addr = NULL;
       hint.ai_next = NULL;
       sprintf(ports, "%d", port);

       err = getaddrinfo(host_server, ports, &hint, &addrlist);
       if (err)
       {
    	   fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(err));
    	   return GET_ADDR_ERROR;
       }
       if (!addrlist || !addrlist->ai_addr)
       {
    	   if (addrlist)
    	   {
    		   freeaddrinfo(addrlist);
    	   }
    	   fprintf(stderr, "getaddrinfo error: addr info is null\n");
    	   return GET_ADDR_ERROR;
       }

       if (addr)
       {
    	   memcpy(addr, (struct sockaddr_in*)(addrlist->ai_addr), sizeof(struct sockaddr_in));
       }
       freeaddrinfo(addrlist);
    } 
    else
    {
    	memset(addr, 0, sizeof(struct sockaddr_in));
        addr->sin_family = AF_INET;
        addr->sin_addr.s_addr = htons(INADDR_ANY);
        addr->sin_port = htons(port);
    }
    return 0;
}

int tcp_stream_server(char *host_server, int port, int non_block, int *fd)
{
    int sock = 0;
    int err = 0;
    struct sockaddr_in addr;
    unsigned int yes = 1;
    
    
    err = get_socket_addr(host_server, port, SOCK_STREAM, &addr);
    if (err)
    {
        fprintf(stderr, "get socket address error\n");
        return err;
    }
    
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
    	fprintf(stderr, "open socket error\n");
        return SOCKET_ERROR;
    }
    err = set_nonblocking(sock, non_block);
    if (err)
    {
        fprintf(stderr, "set socket nonblock error\n");
        close(sock);
        return err;
    }
    
    err = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
    if (err)
    {
        fprintf(stderr, "set socket opt SO_REUSEADDR error\n");
        close(sock);
        return err;
    }
    
    err = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    if (err)
    {
        fprintf(stderr, "bind socket with address error\n");
        close(sock);
        return BIND_ADDR_ERROR;
    }
    
    err = listen(sock, LENGTH_OF_LISTEN_QUEUE);
    if (err)
    {
        fprintf(stderr, "listen socket error\n");
        close(sock);
        return LISTEN_ERROR;
    }

    *fd = sock;
    close(sock);
    return 0;
}

int tcp_stream_accept(int server_sock, int *client_sock)
{
	struct sockaddr_in client_addr;
	int c_sock;
	socklen_t length = sizeof(client_addr);

	c_sock = accept(server_sock, (struct sockaddr*)&client_addr, &length);
	if (c_sock < 0)
	{
		fprintf(stderr, "Server accept failed!\n");
		return ACCEPT_ERROR;
	}

	fprintf(stderr, "Client connected\n");
	*client_sock = c_sock;
	return 0;
}

int tcp_stream_client(char *host_server, int port, int non_block, int *fd)
{
    int sock;
    int err = 0;
    int yes = 1;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    bzero(&client_addr, sizeof(client_addr));
    client_addr.sin_family = AF_INET; //internet协议族
    client_addr.sin_addr.s_addr = htons(INADDR_ANY); //自动获取本机地址
    client_addr.sin_port = htons(0); //系统自动分配一个空闲的端口；

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if(inet_aton(host_server, &server_addr.sin_addr) == 0)
    {
    	fprintf(stderr, "get host ip address error\n");
    	return GET_ADDR_ERROR;
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
    	fprintf(stderr, "open socket error\n");
        return SOCKET_ERROR;
    }

    err = set_nonblocking(sock, non_block);
    if (err)
    {
        fprintf(stderr, "set socket nonblock error\n");
        close(sock);
        return err;
    }

    err = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
    if (err)
    {
        fprintf(stderr, "set socket opt SO_REUSEADDR error\n");
        close(sock);
        return err;
    }

    err = bind(sock, (struct sockaddr *)&client_addr, sizeof(client_addr));
    if (err)
    {
        fprintf(stderr, "bind socket with address error\n");
        close(sock);
        return BIND_ADDR_ERROR;
    }

    err = connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (err)
    {
        fprintf(stderr, "bind socket with address error\n");
        close(sock);
        return CONNECT_ERROR;
    }
    *fd = sock;
    
    return 0;
}

int udp_stream_open()
{
    return 0;
}


int unix_stream_open()
{
    return 0;
}

int tcp_socket_close(int sock)
{
	close(sock);
}
