#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    printf("--beginning of program\n");

    int counter = 0;
    pid_t pid;
    int i;

    for(i = 0; i < 5; i++)
    {
        pid = fork();
        if (pid == 0)
        {
            printf("child process: process id=%d\n", getpid());
            return 0;
        }
        else if(pid < 0)
        {
            printf("fork() failed!\n");
            return -1;
        }
    }

    // parent process
    printf("parent process: process id=%d\n", getpid());
    wait();

    printf("--end of program--\n");

    return 0;
}
