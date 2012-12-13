
#define FIFOPATH "/tmp/mylog.main"
#define FIFOPERMS (S_IRWXU | S_IWGRP| S_IWOTH)

FILE 	*logfile;

void
logger(void *pid)
{

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
