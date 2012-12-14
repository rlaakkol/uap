
#define FIFOPATH "/tmp/mylog.main"
#define FIFOFMT "tmp/mylog.%d"
#define FIFOPERMS (S_IRWXU | S_IWGRP| S_IWOTH)
#define MAXLEN 256
#define TIMEOUT 5

FILE 	*logfile;
pthread_mutex_t 	mutex;

int
output_msg(int readlen, char *buf, char *rem)
{

	int len;
	struct timespec timeout;

	timeout.tv_sec = TIMEOUT;
	timeout.tv_nsec = 0;

	len = strlen(buf);

	if (pthread_mutex_timedlock(mutex, &timeout) != 0 && errno == ETIMEDOUT) {

	fprintf(logfile, "%s", buf);
	fprintf(logfile, "\n");
	pthread_mutex_unlock(mutex);
	return 0;


}

int
get_msg(int fd, char *buf, char delim)
{
	int i, out;

	i = -1;
	while ((out = read(fd, buf + ++i, 1)) && buf[i] != delim);

	return out ? 0 : -1;
}
	


void
logger(void *caller_pid)
{
	int 	pid, logfd, out, readout;
	char 	*fifo, *buf, *rem;
	fd_set 	readfds;

	fifo = malloc(30);
	buf = malloc(MAXLEN);
	rem = malloc(MAXLEN);

	pid = (int) caller_pid;
	snprintf(fifo, FIFOFMT, pid);
	if (mkfifo(fifo, FIFOPERMS) != 0) {

	}
	FD_ZERO(&readfds);
	for (;;) {
		FD_SET(logfd, readfds);
		select(logfd + 1, &readfds, NULL, NULL, NULL);
		readout = get_msg(logfd, buf, '\0');
		while ((out = output_msg(buf)) > 0) {
			readlen -= out;
		}
		if (out == -1) break;


	}


	free(fifo);
	free(buf);
	pthread_exit(NULL);
}

int
start_log(const char *buf)
{
	int 	pid;
	pthread_t 	tid;

	pid = atoi(buf);
	pthread_create(&tid, NULL, &logger, (void *) pid);
}


int
main(int argc, char *argv[])
{
	fd_set 	readfds;
	int 	mainfd, readout;
	char 	buf[BUFLEN];

	if ((logfile = fopen(argv[1], "w")) == NULL) {
		perror("Cannot open logfile for writing");
		exit(-1);
	}
	if (mkfifo(FIFOPATH, FIFOPERMS) != 0) {
		perror("Error creating FIFO pipe");
		exit(-1);
	}

	mainfd = open(FIFOPATH, O_RDONLY);
	FD_ZERO(&readfds);

	for (;;) {
		FD_SET(mainfd, readfds);
		select(mainfd + 1, &readfds, NULL, NULL, NULL);
		readout = read(mainfd, buf, '\0');
		if (readout == 0) start_log(buf);

	}
}		
