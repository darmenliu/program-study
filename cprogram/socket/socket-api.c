#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

enum SA_ERROR {
    SOCKET_ERROR = -1,
    NON_BLOCK_ERROR,
	GET_ADDR_ERROR,
	BIND_ADDR_ERROR,
	LISTEN_ERROR,
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

int get_socket_addr(char *host, int port, int type, struct sockaddr_in *addr)
{
    struct addrinfo *addrlist = 0;
    struct addrinfo hint;
    
    char ports[20];
    int err = 0;
    int family = AF_INET;
    int protocol = 0;
    
    if (host)
    {
       memset(&hint, 0, sizeof(hint)); 
       hint.ai_family = family;
       hint.ai_socktype = type;
       hint.ai_protocol = protocol;
       hint.ai_canonname = NULL;
       hint.ai_addr = NULL;
       hint.ai_next = NULL;
       sprintf(ports, "%d", port);

       err = getaddrinfo(host, ports, &hint, &addrlist);
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
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htons(INADDR_ANY);
        addr.sin_port = htons(port);
    }
    return 0;
}

int tcp_stream_server(char *host, int port, int non_block, int *fd)
{
    int sock = 0;
    int err = 0;
    struct sockaddr_in addr;
    unsigned int yes = 1;
    struct sockaddr_in addr;
    
    
    err = get_socket_addr(host, port, SOCK_STREAM, &addr);
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
    err = set_nonblocking(fd, non_block);
    if (err)
    {
        fprintf(stderr, "set socket nonblock error\n");
        close(fd);
        return err;
    }
    
    err = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
    if (err)
    {
        fprintf(stderr, "set socket opt SO_REUSEADDR error\n");
        close(fd);
        return err;
    }
    
    err = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
    if (err)
    {
        fprintf(stderr, "bind socket with address error\n");
        close(fd);
        return BIND_ADDR_ERROR;
    }
    
    err = listen(fd, LENGTH_OF_LISTEN_QUEUE);
    if (err)
    {
        fprintf(stderr, "listen socket error");
        close(fd);
        return LISTEN_ERROR;
    }
    
    return 0;
}

int tcp_stream_client(int port, int non_block, int *fd)
{
    
}

int udp_stream_open()
{
    
}


int unix_stream_open()
{
    
}
