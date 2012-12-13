
int mainfd_w
int logfd
	
int
mylog_open(char *tag)
{
	int 	pid;
	char 	omsg[30];
	size_t 	len;
	/* Open pipes */
	mainfd_w = open(FIFO_WRITE, O_WRONLY);

	/* Write to mylogd main process pipe */
	pid = getpid();
	sprintf(omsg, "%d:%s", pid, tag);
	write(mainfd_w, omsg, len);
	
	/* Open pid-specific pipe (once it is created) */
	logfd = open(LOGFIFO(pid), O_WRONLY);

}


