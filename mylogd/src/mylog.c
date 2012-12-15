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

#include "mylog.h"

#define FIFOFMT "/tmp/mylog.%d"
#define FIFO_WRITE "/tmp/mylog.main"
#define MAXLEN 256
int daemonfd_w;
int logfd;
char caller[30];
int pid;


int
mylog_open(const char *tag)
{
	char 	omsg[30], path[30];
	size_t 	len;

	strcpy(caller, tag);
	pid = getpid();
	/* Open pipes */
	daemonfd_w = open(FIFO_WRITE, O_WRONLY);

	/* Write to mylogd main process pipe */
	sprintf(omsg, "%d", pid);
	len = strlen(omsg);
	printf("Sending open message: %s\n", omsg);
	len = write(daemonfd_w, omsg, len + 1);
	printf("Wrote %d bytes\n", (int) len);

	
	/* Open pid-specific pipe (once it is created) */
	sprintf(path, FIFOFMT, pid);
	while ((logfd = open(path, O_WRONLY)) == -1) {
		if (errno == EACCES) return -1;
	}

	/* Write opening message */
	mylog_write("Started logging.");

	return 0;

}

int
mylog_write(const char *msg)
{

	char 	buf[MAXLEN], tmpbuf[30];
	struct timeval 	tmp;
	struct tm 	*ltmp;
	size_t 	len;

	gettimeofday(&tmp, NULL);
	ltmp = localtime(&(tmp.tv_sec));
	strftime(tmpbuf, 29, "%b %d %H:%M:%S", ltmp);
	snprintf(buf, MAXLEN, "%s.%ld\t%d:%s\t%s", tmpbuf, tmp.tv_usec, pid, caller, msg);
	len = strlen(buf);
	write(logfd, buf, len + 1);
	return 0;
}

int
mylog_close(void)
{
	write(logfd, "C", 2);
	close(logfd);
	return 0;
}





