#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#define PIPE_STDIN 0
#define PIPE_STDOUT 1
#define MAX_LINE 80

int main(int argc, char **argv)
{
    printf("--beginning of program\n");
    int ret, myPipe[2];
    int counter = 0;
	char buffer[MAX_LINE+1];
	const char *string = "A sample message.";
	
	ret = pipe(myPipe);
	if (ret < 0)
	{
		printf("failed to build the pipe! program exit.\n");
		return 1;
	}
	
    pid_t pid = fork();

    if (pid == 0)
    {
        // child process
		printf("child process: read data from pipe.\n");
		read(myPipe[PIPE_STDIN], buffer, MAX_LINE);
		printf("read data from the pipe, buffer = %s\n", buffer);
    }
    else if (pid > 0)
    {
        // parent process
		printf("parent process: write data from pipe.\n");
		write(myPipe[PIPE_STDOUT], string, strlen(string) + 1);
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
