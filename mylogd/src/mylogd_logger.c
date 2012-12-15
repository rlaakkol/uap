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

pthread_mutex_t 	mutex = PTHREAD_MUTEX_INITIALIZER;

int
output_msg(char *buf)
{

	struct timespec timeout;

	timeout.tv_sec = TIMEOUT;
	timeout.tv_nsec = 0;


	if (pthread_mutex_timedlock(&mutex, &timeout) != 0 && errno == ETIMEDOUT) {
		perror("Failed to acquire lock for writing (timeout occurred).");
		pthread_exit(NULL);
	}

	fprintf(logfile, "%s\n", buf);
	fflush(logfile);
	pthread_mutex_unlock(&mutex);
	return 0;


}

int
get_msg(int fd, char *buf, char delim)
{
	int i, out;

	i = 0;
	while ((out = read(fd, buf + i, 1)) && buf[i] != delim) {
		printf("%c", buf[i]);
		i++;
	}
	printf("\n");

	return out != -1 && buf[i] == delim ? 0 : -1;
}
	


void
*logger(void *caller_pid)
{
	int 	*pid_ptr, pid, logfd;
	char 	*fifo, *buf;
	fd_set 	readfds;

	fifo = malloc(30);
	buf = malloc(MAXLEN);
	
	pid_ptr = (int *) caller_pid;
	pid = *pid_ptr;
	snprintf(fifo, MAXLEN, FIFOFMT, pid);
	if (mkfifo(fifo, FIFOPERMS) != 0) {

	}
	logfd = open(fifo, O_RDWR);
	printf("opened fifo %s\n", fifo);
	for (;;) {
		FD_ZERO(&readfds);
		FD_SET(logfd, &readfds);
		select(logfd + 1, &readfds, NULL, NULL, NULL);
		if(get_msg(logfd, buf, '\0') != 0) {
			perror("Invalid log input");
			continue;
		}
		if (strcmp(buf, "C") == 0) break;
		output_msg(buf);


	}
	
	close(logfd);
	unlink(fifo);

	free(fifo);
	free(buf);
	pthread_exit(NULL);
}

int
start_log(int pid)
{
	pthread_t 	tid;

	pthread_create(&tid, NULL, logger, (void *) &pid);
	printf("created thread for pid %d\n", pid);
	return 0;
}
