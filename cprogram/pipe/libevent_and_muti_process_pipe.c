#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <event.h>
#include <fcntl.h>
#include <errno.h>


#define PIPE_STDIN 0
#define PIPE_STDOUT 1
#define MAX_LINE 1000

int mypipe[2];
struct event_base *base;

void on_read(int fd, short event, void *arg)
{
	//printf("on_reading\n");
	int len;
	char buffer[MAX_LINE+1];
	read(mypipe[PIPE_STDIN], buffer, MAX_LINE);

    if (len <= 0) {
		if (len == -1)
			perror("read");
		else if (len == 0)
			fprintf(stderr, "Connection closed\n");
		event_del(ev);
		event_base_loopbreak(event_get_base(ev));
		return;
	}
    
	printf("read data from the pipe, buffer = %s\n", buffer);
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

int init_pipe()
{
	int ret;
	ret = pipe(mypipe);
	if (ret < 0)
	{
		printf("failed to build the pipe! program exit.\n");
		return -1;
	}
    setnonblock(mypipe[PIPE_STDIN]);
    event_init();
}

void child_process(void *arg)
{
    int write_count = 10;
    int i = 0;
    int size = 0;
	char string[1000];
    sprintf(string, "sample message from process %d\n", getpid());
    // parent process
	//printf("child process: start to write data in pipe. pid=%d\n", getpid());
    //for(i = 0; i < write_count; i++)
    //{
        size = write(mypipe[PIPE_STDOUT], string, strlen(string));
        printf("child process: write data in pipe. size=%d pid=%d\n", size, getpid());
    //}
}

void parent_process(void *arg)
{
	struct event event_read;
    // child process
	printf("parent process: read data from pipe. pid=%d\n", getpid());
	base = event_base_new();
	event_set(&event_read, mypipe[PIPE_STDIN], EV_READ|EV_PERSIST, on_read, NULL);
	event_base_set(base, &event_read);
	event_add(&event_read, NULL);
	
	event_base_dispatch(base);
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
            return 1;
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
    printf("--beginning of program\n");
    int ret;
	
	ret = init_pipe();
	if (ret < 0)
	{
		return -1;
	}
	
    ret = fork_muti_process(100);

    // parent process
    if (ret == 0)
    {
        parent_process(NULL);
        printf("--end of program--\n");
        wait();
    }

    return 0;
}
