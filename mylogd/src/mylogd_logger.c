#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include "mylogd.h"

// Mutex for synchronizing writing to logfile
pthread_mutex_t 	mutex = PTHREAD_MUTEX_INITIALIZER;

/*
 * Write message to logfile
 * */
int
output_msg(char *buf)
{

	struct timespec timeout;

	timeout.tv_sec = TIMEOUT;
	timeout.tv_nsec = 0;

	// Acquire mutex to gain write access to logfile
	if (pthread_mutex_timedlock(&mutex, &timeout) != 0 && errno == ETIMEDOUT) {
		perror("Failed to acquire lock for writing (timeout occurred).");
		return(-1);
	}

	fprintf(logfile, "%s\n", buf);
	fflush(logfile);
	pthread_mutex_unlock(&mutex);
	return 0;


}

/*
 * Read message from fd
 * */
int
get_msg(int fd, char *buf, char delim)
{
	int i, out;

	i = 0;
	// Read message one byte at a time from fd.
	while (i < MAXLEN && (out = read(fd, buf + i, 1)) > 0 && buf[i] != delim) {
		i++;
	}
	if (i < MAXLEN) buf[i] = '\0';
	else buf[MAXLEN - 1] = '\0';
	// Return success if read was successful and ended in delim.
	return i < MAXLEN && out != -1 && buf[i] == delim ? 0 : -1;
}
	
/*
 * Generate name for fifo according to pid.
 * */
char
*fifoname(int pid)
{
	char 	*name;

	name = malloc(30*sizeof(char));
	// Even this didn't make valgrind happy...
	for (int i = 0; i < 30; i++) {
		name[i] = '\0';
	}
	snprintf(name, 28, FIFOFMT, pid);


	return name;
}

/*
 * The main logging functionality of a thread
 * */
void
*logger(void *caller_pid)
{
	int 	*pid_ptr, pid, logfd;
	char 	*fifo, *buf;
	fd_set 	readfds;

	
	// Open process-specific fifo
	pid_ptr = (int *) caller_pid;
	pid = *pid_ptr;
	fifo = fifoname(pid);

	mkfifo(fifo, FIFOPERMS);
	// Open fifo for reading (and writing for select to work).
	logfd = open(fifo, O_RDWR);
	free(fifo);
	for (;;) {
		FD_ZERO(&readfds);
		FD_SET(logfd, &readfds);
		// Wait for incoming bytes
		select(logfd + 1, &readfds, NULL, NULL, NULL);
		// Allocate message buffer
		buf = malloc(MAXLEN);
		// Read message from fd
		if(get_msg(logfd, buf, '\0') != 0) {
			output_msg("Invalid log input");
			free(buf);
			continue;
		}
		// If message is "close" command, stop logging
		if (strcmp(buf, CLOSE_MSG) == 0) {
			free(buf);
			break;
		}
		// Write to logfile
		output_msg(buf);
		free(buf);


	}

	// cleanup
	
	close(logfd);
	fifo = fifoname(pid);
	unlink(fifo);

	free(fifo);
	pthread_exit(NULL);
}

/*
 * Create logger thread
 * */
int
start_log(int pid)
{
	pthread_t 	tid;
	// Create thread, pass caller process pid as argument.
	pthread_create(&tid, NULL, logger, (void *) &pid);
	// Detach thread immediately to free resources after it's done
	pthread_detach(tid);
	return 0;
}
