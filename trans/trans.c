#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include "morse.h"

volatile int	sig_flag;
volatile int 	ready_flag;
FILE 	*logfile;

/*
void
handler(int signo)
{
	if (signo == MORSE_SHORT) sig_flag = MORSE_SHORT;
	else if (signo == MORSE_LONG) sig_flag = MORSE_LONG;
	else if (signo == MORSE_PAUSE) sig_flag = MORSE_PAUSE;
	else sig_flag = MORSE_ERR;
}
*/
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

	fprintf(logfile, "quitting child\n");
	fclose(infile);
	return 0;
}

int
parent_func(char *outfilename, FILE *outfile, int cpid)
{


	char	nextc;

	if ((outfile = fopen(outfilename, "w")) == NULL) {
		perror("fopen error");
		exit(-1);
	}
	sig_sleep(1, 0);
	while (nextc != EOF) {
		nextc = get_char(cpid);
		if (nextc != EOF) fputc(nextc, outfile);
	}

	fprintf(logfile, "quitting parent\n");
	fclose(outfile);
	return 0;
}

int
main(int argc, char** argv)
{
	int 			pid;
	char 			*infilename, *outfilename;
	FILE 			*infile, *outfile;
/*	struct sigaction 	sa; */
	sigset_t 		ss;
		

	infilename = argv[1];
	outfilename = argv[2];
	if (argc > 3) logfile = fopen(argv[3], "w");
	else logfile = stdout;
	

	sig_flag = MORSE_PAUSE;


	sigemptyset(&ss);
	sigaddset(&ss, MORSE_SHORT);
	sigaddset(&ss, MORSE_LONG);
	sigaddset(&ss, MORSE_PAUSE);
/**
	sa.sa_mask = ss;

	sa.sa_handler = &handler;
	sigaction(MORSE_SHORT, &sa, NULL);
	sigaction(MORSE_LONG, &sa, NULL);
	sigaction(MORSE_PAUSE, &sa, NULL);
**/	
	sigprocmask(SIG_BLOCK, &ss, NULL);

	if ((pid = fork()) < 0){
		perror("fork error");
		exit(-1);
	}
	if (pid == 0) {
		/* In child */
		child_func(infilename,infile);
	}
	else {
		/* In parent */
		parent_func(outfilename, outfile, pid);
	}
	fclose(logfile);
	return 0;
}






		
