#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>

enum SA_ERROR {
    SOCKET_ERROR = -1,
    NON_BLOCK_ERROR,
}

int set_nonblocking(int fd)
{
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
       
       sprintf(ports, %d, );
    } 
    else
    {
        
    }
}

int tcp_stream_open(int port, bool non_block, int *fd)
{
    int fd;
    int error = 0;
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htons(INADDR_ANY);
    addr.sin_port = htons(port);
    
    fd = socket(PF_INET, SOCKET_STREAM, 0);
    if (fd < 0)
    {
        error = errno;
        return error;
    }
    
    if (non_block)
    {
        error = set_nonblocking(fd);
        if (error)
        {
            close(fd);
            return error;
        }
    }
    
    
    
    
    
    
}

int udp_stream_open()
{
    
}


int unix_stream_open()
{
    
}
