#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <assert.h>

#include "mylogd.h"

FILE *logfile;

void
print_usage(void)
{

}

void closeall(void) {
	int fd = 0;
	int fdlimit = sysconf(_SC_OPEN_MAX);
	while (fd < fdlimit)
		close(fd++);
}

void
unlinkall(void)
{


}

void
sigint_handler(int signo)
{
	if (signo == SIGINT) {
		closeall();
		unlink(FIFOPATH);
		unlinkall();
	}
	
}

int
main(int argc, char *argv[])
{
	fd_set 			readfds;
	int 			mainfd, readout, sout;
	char 			buf[MAXLEN];
	struct sigaction 	sig;
	
	sigemptyset(&sig.sa_mask);
	sig.sa_flags=0;
	sig.sa_handler = sigint_handler;
	assert((sigaction(SIGINT,&sig,NULL)) == 0);

	if (argc != 2) {
		print_usage();
	}
	if ((logfile = fopen(argv[1], "w")) == NULL) {
		perror("Cannot open logfile for writing");
		exit(-1);
	}
	if (mkfifo(FIFOPATH, FIFOPERMS) != 0) {
		perror("Error creating FIFO pipe");
		exit(-1);
	}

	mainfd = open(FIFOPATH, O_RDWR);

	for (;;) {
		FD_ZERO(&readfds);
		FD_SET(mainfd, &readfds);
		sout = select(mainfd + 1, &readfds, NULL, NULL, NULL);
		if (sout < 0) {
			perror("Select error: ");
			break;
		}
		readout = get_msg(mainfd, buf, '\0');
		printf("Received message: %s, Readout is %d\n", buf, readout);
		if (readout == 0) start_log(atoi(buf));


	}

	close(mainfd);
	unlink(FIFOPATH);
	return 0;
}		
