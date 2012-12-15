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
#include <dirent.h>
#include <fnmatch.h>

#include "mylogd.h"

FILE *logfile;

void
print_usage(void)
{
	printf("Usage: daemon/mylogd <absolute path to logfile>\n");
}

/*
 * Close all open descriptors. Copied directly from Lecture 4 Example code
 * */
void closeall(void) {
	int fd = 0;
	int fdlimit = sysconf(_SC_OPEN_MAX);
	while (fd < fdlimit)
		close(fd++);
}

/*
 * Function for removing remaining fifos
 * */
void
unlinkall(void)
{
	DIR *dp;
	struct dirent *ep;

	dp = opendir("/tmp");
	if (dp != NULL)
	{
		chdir("/tmp");
		// Scan through /tmp and remove anything starting with "mylog."
		while ((ep = readdir(dp))) {
			if (!fnmatch("mylog.*", ep->d_name, 0)) {
				unlink(ep->d_name);
			}
		}
			
		(void) closedir(dp);
	}

}

/*
 * Handle SIGINT.
 * Close open descriptors, remove remaining fifos
 * */
void
sigint_handler(int signo)
{
	if (signo == SIGINT) {
		closeall();
		unlink(FIFOPATH);
		unlinkall();
		fclose(logfile);
	}
	
}

/* 
 * Daemonize process. Copied directly from Lecture 4 Example code (daemon.c)
 * */
int
turn_into_daemon(void) {
	// Fork again to sever ties to parent.
	switch (fork())  {
		case 0:  break;     /* We're the child, ok. */
		case -1: return -1; /* Fork failed! */
		default: _exit(0);  /* Parent exits. */
	}


	// Get a new session.
	assert(setsid() > 0);               /* shouldn't fail */

	/* Comment out this switch if you want to acquire a control tty in
	 * the future -- not normally advisable for daemons. */
	switch (fork()) {
		case 0:  break;
		case -1: return -1;
 		default: _exit(0);
	}


	/* Get to the root directory, just in case the current working
	 * directory needs to be unmounted at some point. */
	chdir("/");

	/* Close all open files. */
	closeall();

	/* open /dev/null. As all fds are closed, the following open
	 * will make stdin=/dev/null  */
	open("/dev/null",O_RDWR);
	dup(0);  /* Copy /dev/null also as stdout stream */
	dup(0);  /* Copy /dev/null also as stderr stream */

	// Now we're a daemon, a lonely avenger fighting for the cause.

	return 0;
}

int
main(int argc, char *argv[])
{
	fd_set 			readfds;
	int 			mainfd, readout, sout;
	char 			buf[MAXLEN_OPEN];
	struct sigaction 	sig;
	
	if (argc != 2) {
		print_usage();
		exit(-1);
	}

	// Remove conflicting fifos
	unlinkall();

	turn_into_daemon();
	
	// Setup signal handler for SIGINT
	sigemptyset(&sig.sa_mask);
	sig.sa_flags=0;
	sig.sa_handler = sigint_handler;
	assert((sigaction(SIGINT,&sig,NULL)) == 0);

	if ((logfile = fopen(argv[1], "w")) == NULL) {
		perror("Cannot open logfile for writing");
		exit(-1);
	}
	// Create fifo for incoming "open log" requests
	if (mkfifo(FIFOPATH, FIFOPERMS) != 0) {
		perror("Error creating FIFO pipe");
		exit(-1);
	}
	
	// Open fifo for reading (and writing for select to work correctly)
	mainfd = open(FIFOPATH, O_RDWR);

	for (;;) {
		FD_ZERO(&readfds);
		FD_SET(mainfd, &readfds);
		// Wait for incoming bytes from fifo
		sout = select(mainfd + 1, &readfds, NULL, NULL, NULL);
		if (sout < 0) {
			perror("Select error: ");
			break;
		}
		// Read from fifo
		readout = get_msg(mainfd, buf, '\0');
		// Start log for the client process
		if (readout == 0) start_log(atoi(buf));


	}

	close(mainfd);
	unlink(FIFOPATH);
	return 0;
}		
