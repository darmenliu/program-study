#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <event.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>



#define PIPE_STDIN 0
#define PIPE_STDOUT 1
#define MAX_LINE 80
#define MY_FIFO "/tmp/example_message_fifo"

struct event_base *base;
int read_fd;

static void
signal_cb(evutil_socket_t fd, short event, void *arg)
{
	struct event_base *base = arg;
	event_base_loopbreak(base);
}


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

void on_read(int fd, short event, void *arg)
{
	int len;
	char buffer[MAX_LINE+1];
    struct event *ev = arg;
    
	len = read(read_fd, buffer, MAX_LINE);

/*
    if (len <= 0) {
		if (len == -1)
			perror("read");
		else if (len == 0)
			fprintf(stderr, "Connection closed\n");
		event_del(ev);
		event_base_loopbreak(event_get_base(ev));
		return;
	}
*/

    
	printf("read data from the pipe, buffer = %s\n", buffer);

}

int init_pipe()
{
	int ret;
    if (access(MY_FIFO, F_OK) == -1)
    {
    	ret = mkfifo(MY_FIFO, 0600);
    	if (ret < 0)
    	{
    		printf("failed to build the pipe %s! program exit error %s.\n", MY_FIFO, strerror(errno));
    		return -1;
    	}
    }
    return 0;
}


int parent_process(void *arg)
{
    int write_count = 10;
    int i = 0;
	const char *string = "A sample message.";
    int open_mode = O_WRONLY;
    int pipe_fd;

    printf("parent Process %d opening FIFO O_WRONLY\n", getpid());  
    pipe_fd = open(MY_FIFO, open_mode);

    if (pipe_fd < 0)
    {
        printf("failed to open FIFO file %s\n ", MY_FIFO);
        return -1;
    }
    setnonblock(pipe_fd);
    // parent process
	printf("parent process: start to write data in pipe.\n");
    for(i = 0; i < write_count; i++)
    {
    	sleep(2);
        write(pipe_fd, string, strlen(string) + 1);
        printf("parent process:write buffer success %d\n", i);
    }

    close(pipe_fd);
	wait();
    return 0;
}

int child_process(void *arg)
{
	struct event *event_read;
    int open_mode = O_RDWR;
    struct event *signal_int;
    printf("child Process %d opening FIFO O_RDONLY\n", getpid()); 

    read_fd = open(MY_FIFO, open_mode);
    if (read_fd < 0)
    {
        printf("failed to open FIFO file %s\n ", MY_FIFO);
        return -1;
    }
    setnonblock(read_fd);
    // child process
	printf("child process: read data from pipe.\n");
	base = event_base_new();

    //signal_int = evsignal_new(base, SIGINT, signal_cb, base);
	//event_add(signal_int, NULL);
    
	event_read = event_new(base, read_fd, EV_READ|EV_PERSIST, on_read,NULL);
	//event_base_set(base, &event_read);
	event_add(event_read, NULL);
	
	event_base_dispatch(base);
    event_base_free(base);
    close(read_fd);
    return 0;
}

int main(int argc, char **argv)
{
    printf("--beginning of program\n");
    int ret;
	
	ret = init_pipe();
	if (ret < 0)
	{
		return -1;
	}
	
    pid_t pid = fork();

    if (pid == 0)
    {
        // child process
    	child_process(NULL);
    }
    else if (pid > 0)
    {
        // parent process
    	parent_process(NULL);
    }
    else
    {
        // fork failed
        printf("fork() failed!\n");
        return 1;
    }

    printf("--end of program--\n");

    return 0;
}
