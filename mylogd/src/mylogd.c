
#define FIFOPATH "/tmp/mylog.main"
#define FIFOPERMS (S_IRWXU | S_IWGRP| S_IWOTH)
FILE 	*logfile;


int
main(int argc, char *argv[])
{
	fd_set 	readfds;
	int 	maind;

	if ((logfile = fopen(argv[1], "w")) == NULL) {
		perror("Cannot open logfile for writing");
		exit(-1);
	}
	if ((mainfd = mkfifo(FIFOPATH, FIFOPERMS;

	for (;;) {

	}
		
