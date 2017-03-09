#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <event.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>




#define PIPE_STDIN 0
#define PIPE_STDOUT 1
#define MAX_LINE 2048
#define MY_FIFO "/tmp/example_message_fifo"

struct event_base *base;
int read_fd;

int setnonblock(int fd)
{
	int flags;

	flags = fcntl(fd, F_GETFL);
	if (flags < 0)
		return flags;
	flags |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, flags) < 0)
		return -1;

        return 0;
}

static void
signal_cb(evutil_socket_t fd, short event, void *arg)
{
	struct event_base *base = arg;
	event_base_loopbreak(base);
}


void on_read(int fd, short event, void *arg)
{
	//printf("on_reading\n");
	int length;
	char buffer[MAX_LINE+1];
    struct event *ev = arg;
    memset(buffer, 0, sizeof(char) * (MAX_LINE + 1));
    
	length = read(read_fd, buffer, MAX_LINE);

	if (length <= 0) {
		if (length == -1)
			perror("read");
		else if (length == 0)
			fprintf(stderr, "Connection closed\n");
		event_del(ev);
		event_base_loopbreak(event_get_base(ev));
		return;
	}

    buffer[length] = '\0';
    printf("---------------------------\n");
	printf("%s\n", buffer);
    printf("---------------------------\n");
}

int init_pipe()
{
	int ret;
    if (access(MY_FIFO, F_OK) == -1)
    {
    	ret = mkfifo(MY_FIFO, 0777);
    	if (ret < 0)
    	{
    		printf("failed to build the pipe %s! program exit error %s.\n", MY_FIFO, strerror(errno));
    		return -1;
    	}
    }
    event_init();
    return 0;
}


int child_process(void *arg)
{
    int write_count = 0;
    int i = 0;
    int open_mode = O_WRONLY;
    int pipe_fd;
    //printf("child process %d\n", getpid());

    char string[1024];
    sprintf(string, "\ntest request string %d\n", getpid());

    //printf("child Process %d opening FIFO O_WRONLY\n", getpid());  
    pipe_fd = open(MY_FIFO, open_mode);

    if (pipe_fd < 0)
    {
        printf("failed to open FIFO file %s\n ", MY_FIFO);
        return -1;
    }
//    setnonblock(pipe_fd);
    // parent process
	//printf("child process: start to write data in pipe.\n");
    //for(i = 0; i < write_count; i++)
    {
        //sleep(1);
        write_count = write(pipe_fd, string, strlen(string));
        if(write_count < 0)
        {
            printf("child process:write buffer failed, process = %d\n", i, getpid());
        } else if (write_count < strlen(string))
        {
            printf("child process:write buffer less than string length, process = %d\n", i, getpid());
        }
        //printf("child process:write buffer success %d, process = %d\n", i, getpid());
    }

    close(pipe_fd);
    return 0;
}

int parent_process(void *arg)
{
	struct event event_read;
    struct event *signal_int;
    int open_mode = O_RDONLY;
    //printf("parent Process %d opening FIFO O_RDONLY\n", getpid()); 

    read_fd = open(MY_FIFO, open_mode);
    if (read_fd < 0)
    {
        printf("parent process failed to open FIFO file %s\n ", MY_FIFO);
        return -1;
    }
    setnonblock(read_fd);
    // child process
	//printf("child process: read data from pipe.\n");
	base = event_base_new();
//    signal_int = evsignal_new(base, SIGINT, signal_cb, base);
//	event_add(signal_int, NULL);
	event_set(&event_read, read_fd, EV_READ|EV_PERSIST, on_read, &event_read);
	base = NULL;
	event_base_set(base, &event_read);
	event_add(&event_read, NULL);
	
	event_base_dispatch(base);
    close(read_fd);
    wait();
    return 0;
}

int fork_muti_process(int count)
{
    int i;
    int pid;

    for (i = 0; i < count; i++)
    {
        pid = fork();
        if (pid == 0)
        {
            child_process(NULL);
            exit(1);
        }
        else if (pid < 0)
        {
            printf("fork() failed!\n");
            return -1;
        }
    }
    return 0;
}

int main(int argc, char **argv)
{
    //printf("--beginning of program\n");
    int ret;
	
	ret = init_pipe();
	if (ret < 0)
	{
		return -1;
	}
	
    ret = fork_muti_process(300);

    // parent process
    if (ret == 0)
    {
        parent_process(NULL);
        //printf("--end of program--\n");
    }

    return 0;
}
