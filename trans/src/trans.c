#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <time.h>

#include "morse.h"

FILE 	*logfile;

/* Usage message to stdout */
void
print_usage(void)
{
	printf("Usage: ./trans <input file> <output file> [<logfile>]\n");
	printf("(Use \"stdout\" for output file name for stdout)\n");
}

/** Operations performed by child process:
 * open input file and morse signal it character
 * by character to parent process.
 * */
int
child_func(char *filename, FILE* infile)
{
	int 	ppid;
	char 	c;

	/* Open input file */
	if ((infile = fopen(filename, "r")) == NULL) {
		perror("error opening input file");
		print_usage();
		exit(-1);
	}

	ppid = getppid();
	/* Loop until EOF occurs */
	do {
		/* Read char from file */
		c = getc(infile);
		/* signal to parent */
		sig_char(ppid, c);

	} while (c != EOF);

	fprintf(logfile, "%ld: quitting child\n", (long int)time(NULL));
	fclose(infile);
	return 0;
}

/** Operations performed by parent process:
 * open output file and listen to character signals
 * from child processa and output them to the file
 */
int
parent_func(char *outfilename, FILE *outfile, int cpid)
{
	char	nextc;

	/* Open output file */
	if (strcmp(outfilename, "stdout") == 0) outfile = stdout;
	else if ((outfile = fopen(outfilename, "w")) == NULL) {
		perror("error opening output file");
		print_usage();
		exit(-1);
	}
	/* Loop until EOF occurs */
	nextc = 1;
	while (nextc != EOF) {
		nextc = get_char(cpid);
		/* Write to file (unless EOF) */
		if (nextc != EOF) fputc(nextc, outfile);
		/* FLush after each character */
		fflush(outfile);
	}

	fclose(outfile);
	fprintf(logfile, "%ld: quitting parent\n", (long int)time(NULL));
	return 0;
}

/** The main function reads command line arguments, opens
 * the log and forks the child process.
 */
int
main(int argc, char** argv)
{
	int 			pid;
	char 			*infilename, *outfilename;
	FILE 			*infile, *outfile;
	sigset_t 		ss;

	infile = NULL;
	outfile = NULL;
	
	if (argc < 3 || argc > 4) {
		print_usage();
		exit(-1);
	}

	infilename = argv[1];
	outfilename = argv[2];
	if (argc > 3) { 
		if ((logfile = fopen(argv[3], "w")) == NULL) {
			perror("unable to open logfile");
			print_usage();
			exit(-1);
		}
	} else logfile = stdout;
	fprintf(logfile, "%ld: log opened\n", (long int)time(NULL));

	/** Create a blocking sigprocmask for the morse code signals so 
	 * that the blocked signals can be caught with sigtimedwait 
	 * instead of the signal handler
	 * */
	sigemptyset(&ss);
	sigaddset(&ss, MORSE_SHORT);
	sigaddset(&ss, MORSE_LONG);
	sigaddset(&ss, MORSE_PAUSE);
	
	sigprocmask(SIG_BLOCK, &ss, NULL);
	
	/* Fork the child process */
	if ((pid = fork()) < 0){
		perror("fork error");
		exit(-1);
	}
	if (pid == 0) {
		/* In child */
		child_func(infilename,infile);
		fflush(logfile);
		fclose(logfile);
	}
	else {
		/* In parent */
		parent_func(outfilename, outfile, pid);
		fflush(logfile);
		fclose(logfile);
	}
	return 0;
}






		
