#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>

#include "mylog.h"

int daemonfd_w;
int logfd;
char caller[30];
int pid;
pthread_mutex_t         mutex = PTHREAD_MUTEX_INITIALIZER;


/*
 * Open log and log starting message
 * */
int
mylog_open(const char *tag)
{
	char 	omsg[20], path[30];
	size_t 	len;

	strncpy(caller, tag, 29);
	caller[30] = '\0';
	pid = getpid();
	/* Open the "log request" pipe for writing */
	daemonfd_w = open(FIFOPATH, O_WRONLY);

	/* Write to open request to mylogd main process pipe */
	sprintf(omsg, "%d", pid);
	len = strlen(omsg);
	len = write(daemonfd_w, omsg, len + 1);

	
	/* Open pid-specific pipe (once it is created) */
	sprintf(path, FIFOFMT, pid);
	while ((logfd = open(path, O_WRONLY)) == -1) {
		if (errno == EACCES) return -1;
	}

	/* Write opening message */
	mylog_write(START_MSG);

	return 0;

}

/*
 * Write to log
 * */
int
mylog_write(const char *msg)
{

	char 	buf[MAXLEN], tmpbuf[30];
	struct timeval 	tmp;
	struct tm 	*ltmp;
	size_t 	len;
	
	// Lock mutex for thread safety
	pthread_mutex_lock(&mutex);
	
	// Create timestamp.
	gettimeofday(&tmp, NULL);
	ltmp = localtime(&(tmp.tv_sec));
	strftime(tmpbuf, 29, "%b %d %H:%M:%S", ltmp);
	// Format message to <timestamp><tab><pid>:<caller_tag><tab><msg>
	snprintf(buf, MAXLEN - 1, "%s.%.0f\t%d:%s\t%s", tmpbuf, tmp.tv_usec/1000.0, pid, caller, msg);
	buf[MAXLEN - 1] = '\0';
	len = strlen(buf);
	// Write into log pipe
	write(logfd, buf, len + 1);
	pthread_mutex_unlock(&mutex);
	return 0;
}

/*
 * Close the log so the daemon may cleanup
 * */
int
mylog_close(void)
{
	write(logfd, CLOSE_MSG, 2);
	close(logfd);
	return 0;
}





