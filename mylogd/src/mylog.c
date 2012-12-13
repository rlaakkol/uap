
#define FIFOFMT "/tmp/mylog.%d"
#define FIFO_WRITE "/tmp/mylog.main"
#define MAXLEN 256
int daemonfd_w;
int logfd;
char caller[30];
int pid;

char
*logfmt(struct timeval *tstamp, char *msg)
{
	char buf[

}

int
mylog_open(char *tag)
{
	int 	pid;
	char 	omsg[30], path[30];
	size_t 	len;

	caller = tag;
	pid = getpid();
	/* Open pipes */
	daemonfd_w = open(FIFO_WRITE, O_WRONLY);

	/* Write to mylogd main process pipe */
	sprintf(omsg, "%d", pid);
	len = strlen(omsg);
	write(mainfd_w, omsg, len + 1);
	
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
	ltmp = localtime(tmp.tv_sec);
	strftime(tmpbuf, 29, "%b %d %T", ltmp);
	snprintf(buf, MAXLEN, "%s.%ld\t%d:%s\t%s", tmpbuf, tmp.tv_usec, pid, caller, msg);
	len = strlen(buf);
	write(logfd, buf, len + 1);
	return 0;
}

int
mylog_close(void)
{
	write(logfd, "C", 2);
	return 0;
}





