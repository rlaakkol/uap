#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include "morse.h"

int	sig_flag;

void
handler(int signo)
{
	int	pid;

	pid = getpid();

	if (pid == 0) {
		/* In child, continue */
	} else {
		if (signo == MORSE_SHORT) sig_flag = MORSE_SHORT;
		else if (signo == MORSE_LONG) sig_flag = MORSE_LONG;
		else if (signo == MORSE_PAUSE) sig_flag = MORSE_PAUSE;
		else sig_flag = MORSE_ERR;
	}
}

int
child_func(char *filename, FILE* infile)
{
	int 	ppid;
	char 	c, code[8];

	if ((infile = fopen(filename, "r")) == NULL) {
		perror("error opening input file");
		exit(-1);
	}

	ppid = getppid();
	while ((c = getc(infile)) != EOF) {
		morse_encode(code, c);
		sig_char(ppid, code);

	}

	return 0;
}

int
parent_func(FILE *outfile, int cpid)
{
	char	nextc;
	char	code[8];
	int	i;

	while (nextc != EOF) {
		if (sig_flag == INIT_FLAG) {
			sig_sleep();
		}
		code[0] = sig_to_morse(sig_flag);
		i = 1;
		while (sig_flag != MORSE_PAUSE) {
			kill(cpid, sig_flag);
			sig_sleep();
			code[i] = sig_to_morse(sig_flag);
			i++;
		}
		nextc = morse_decode(code);
		fputc(nextc, outfile);
		sig_flag = INIT_FLAG;
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

	sig_flag = INIT_FLAG;

	sa.sa_handler = &handler;

	sigemptyset(&ss);
	sigaddset(&ss, MORSE_SHORT);
	sigaddset(&ss, MORSE_LONG);
	sigaddset(&ss, MORSE_PAUSE);

	sa.sa_mask = ss;

	sigaction(MORSE_SHORT, &sa, NULL);
	sigaction(MORSE_LONG, &sa, NULL);
	sigaction(MORSE_PAUSE, &sa, NULL);

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
		parent_func(outfile, pid);
	}
	fclose(infile);
	fclose(outfile);
	return 0;
}






		
