/*
 * This sample code shows how to use Libevent to read from a named pipe.
 * XXX This code could make better use of the Libevent interfaces.
 *
 * XXX This does not work on Windows; ignore everything inside the _WIN32 block.
 *
 * On UNIX, compile with:
 * cc -I/usr/local/include -o event-read-fifo event-read-fifo.c \
 *     -L/usr/local/lib -levent
 */

#include <event2/event-config.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/queue.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <event2/event.h>

const char *fifo = "/tmp/event.fifo";

static void
fifo_read(evutil_socket_t fd, short event, void *arg)
{
	char buf[255];
	int len;
	struct event *ev = arg;

	fprintf(stderr, "fifo_read called with fd: %d, event: %d, arg: %p\n",
	    (int)fd, event, arg);
	len = read(fd, buf, sizeof(buf) - 1);

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

	buf[len] = '\0';
	fprintf(stdout, "Read: %s\n", buf);
}

/* On Unix, cleanup event.fifo if SIGINT is received. */
static void
signal_cb(evutil_socket_t fd, short event, void *arg)
{
	struct event_base *base = arg;
	event_base_loopbreak(base);
}

int init_fifo()
{
    struct stat st;

    if ((0 == stat(fifo, &st)) && (S_IFIFO == (st.st_mode & S_IFMT))) {
        printf("init fifo, file exist");
        return -1;
	}
    printf("start mkfifo");
	//unlink(fifo);
	if (mkfifo(fifo, 0600) == -1) {
		printf("Mkfifo failed");
		return -1;
	}
    return 0;
}

int write_process()
{
	int socket, i;
    int size;
    const char *string = "A sample message.";
    socket = open(fifo, O_RDWR | O_NONBLOCK, 0);

	if (socket == -1) {
		perror("open");
		return -1;
	}

	fprintf(stderr, "Write data to %s\n", fifo);

    for(i = 0; i < 5; i++)
    {
        sleep(2);
        size = write(socket, string, strlen(string) + 1);
        fprintf(stderr, "Write data size %d\n", size);
    }

    close(socket);

    return 0;
}

int read_process()
{
    struct event *evfifo;
	struct event_base* base;
	struct event *signal_int;
    int socket;

    socket = open(fifo, O_RDWR | O_NONBLOCK, 0);

	if (socket == -1) {
		perror("open");
		return -1;
	}

    	/* Initalize the event library */
	base = event_base_new();

	/* Initalize one event */
	/* catch SIGINT so that event.fifo can be cleaned up */
	signal_int = evsignal_new(base, SIGINT, signal_cb, base);
	event_add(signal_int, NULL);

	evfifo = event_new(base, socket, EV_READ|EV_PERSIST, fifo_read,NULL);

	/* Add it to the active events, without a timeout */
	event_add(evfifo, NULL);

	event_base_dispatch(base);
	event_base_free(base);

	close(socket);
    
}

int main(int argc, char **argv)
{

    int ret=0;
	
    int size;

    ret = init_fifo();
    if(ret < 0 )
    {
        exit(1);
    }

    pid_t pid = fork();

    if (pid == 0)
    {
        // child process
    	write_process();
        exit(1);
    }
    else if (pid > 0)
    {
        // parent process
    	read_process();
    }
    else
    {
        // fork failed
        printf("fork() failed!\n");
        return 1;
    }


	//unlink(fifo);
    //libevent_global_shutdown();
	return (0);
}

