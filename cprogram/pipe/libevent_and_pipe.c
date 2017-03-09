#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <event.h>

#define PIPE_STDIN 0
#define PIPE_STDOUT 1
#define MAX_LINE 80

int mypipe[2];
struct event_base *base;

void on_read(int fd, short event, void *arg)
{
	printf("on_reading\n");
	char buffer[MAX_LINE+1];
	read(mypipe[PIPE_STDIN], buffer, MAX_LINE);
	printf("read data from the pipe, buffer = %s\n", buffer);
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
}

void parent_process(void *arg)
{
    int write_count = 10;
    int i = 0;
	const char *string = "A sample message.";
    // parent process
	printf("parent process: start to write data in pipe.\n");
    for(i = 0; i < write_count; i++)
    {
    	sleep(2);
        write(mypipe[PIPE_STDOUT], string, strlen(string) + 1);
    }

	wait();
}

void child_process(void *arg)
{
	struct event event_read;
    // child process
	printf("child process: read data from pipe.\n");
	base = event_base_new();
	event_set(&event_read, mypipe[PIPE_STDIN], EV_READ|EV_PERSIST, on_read, NULL);
	event_base_set(base, &event_read);
	event_add(&event_read, NULL);
	
	event_base_dispatch(base);
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
