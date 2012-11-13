#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include "morse.h"

int	sig_flag;

void
child_handler(int signo)
{
	printf("child received signal %s\n", strsignal(signo));
}

void
parent_handler(int signo)
{
	printf("parent received signal %s\n", strsignal(signo));
	if (signo == MORSE_SHORT) sig_flag = MORSE_SHORT;
	else if (signo == MORSE_LONG) sig_flag = MORSE_LONG;
	else if (signo == MORSE_PAUSE) sig_flag = MORSE_PAUSE;
	else sig_flag = MORSE_ERR;
}

int
child_func(char *filename, FILE* infile)
{
	int 	ppid;
	char 	c;

	if ((infile = fopen(filename, "r")) == NULL) {
		perror("error opening input file");
		exit(-1);
	}

	ppid = getppid();
	do {
		c = getc(infile);
		sig_char(ppid, c);

	} while (c != EOF);


	return 0;
}

int
parent_func(FILE *outfile, int cpid)
{
	char	nextc;

	while (nextc != EOF) {
		nextc = get_char(cpid);
		
		fputc(nextc, outfile);
		kill(cpid, MORSE_PAUSE);
	}

	return 0;
}

int
main(int argc, char** argv)
{
	int 			pid;
	char 			*infilename, *outfilename;
	FILE 			*infile, *outfile;
	struct sigaction 	sa;
	sigset_t 		ss;

	infilename = argv[1];
	outfilename = argv[2];

	if ((infile = fopen(outfilename, "w")) == NULL) {
		perror("fopen error");
		exit(-1);
	}
	outfile = NULL;

	sig_flag = INIT_FLAG;
	parent_hold = 1;
	child_hold = 1;


	sigemptyset(&ss);
	sigaddset(&ss, MORSE_SHORT);
	sigaddset(&ss, MORSE_LONG);
	sigaddset(&ss, MORSE_PAUSE);

	sa.sa_mask = ss;


	if ((pid = fork()) < 0){
		perror("fork error");
		exit(-1);
	}
	if (pid == 0) {
		/* In child */

		sa.sa_handler = &child_handler;
		sigaction(MORSE_SHORT, &sa, NULL);
		sigaction(MORSE_LONG, &sa, NULL);
		sigaction(MORSE_PAUSE, &sa, NULL);
		child_func(infilename,infile);
	}
	else {
		/* In parent */
		sa.sa_handler = &parent_handler;
		sigaction(MORSE_SHORT, &sa, NULL);
		sigaction(MORSE_LONG, &sa, NULL);
		sigaction(MORSE_PAUSE, &sa, NULL);
		parent_func(outfile, pid);
	}
	fclose(infile);
	fclose(outfile);
	return 0;
}






		
