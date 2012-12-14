
#define FIFOPATH "/tmp/mylog.main"
#define FIFOFMT "tmp/mylog.%d"
#define FIFOPERMS (S_IRWXU | S_IWGRP| S_IWOTH)
#define MAXLEN 256

FILE 	*logfile;

int
output_msg(int readlen, char *buf, char *rem)
{
	int 	i;

	i = 0;

	while (i < readlen && buf[i] != '\0')

}
void
logger(void *caller_pid)
{
	int 	pid, logfd, out;
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
		readlen = read(logfd, buf, BUFLEN);
		while ((out = output_msg(readlen, buf, rem)) > 0) {
			readlen -= out;
		}
		if (out == -1) break;


	}


	free(fifo);
	free(buf);
	pthread_exit(NULL);
}

int
start_logs(const char *buf)
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
	int 	mainfd;
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
		readlen = read(mainfd, buf, BUFLEN);
		start_logs(buf);

	}
}		
