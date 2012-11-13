#include "morse.h"
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <error.h>
#include <errno.h>


const char codes[MORSE_NCHARS][MORSE_LEN] = MORSE_CODES;
const char char_map[MORSE_NCHARS] = MORSE_MAP;

volatile int     sig_flag; 
FILE 	*logfile;


int
sig_sleep(time_t sec, long nsec)
{
	struct timespec	sleeptime;

	sleeptime.tv_sec = sec;
	sleeptime.tv_nsec = nsec;

	if (nanosleep(&sleeptime, NULL) != -1) {
		return -1;
	}
	return 0;
}


int
morse_encode(char *buf, char c)
{
	char 	cc;
	int 	i;


	cc = tolower(c);
	i = 0;
	while (i < 46 && char_map[i] != cc) {
		i++;
	}
	

	strcpy(buf, codes[i]);
	return 1;
}

char
morse_decode(char *code)
{
	int i;

	
	i = 0;
	while (i < 47 && !strcmp(code, codes[i])) {
		i++;
	}
	fprintf(logfile, "i = %d, char = %c\n", i, char_map[i]);

	
	return char_map[i];
}

char
sig_to_morse(int sig)
{
	if (sig == MORSE_SHORT) return '.';
	else if (sig == MORSE_LONG) return '-';
	else if (sig == MORSE_PAUSE) return '\0';
	else {
		exit(-1);
		return 0;
	}
}

int
sig_char(int pid, char c)
{
	char	code[8], next;
	int	i;
	

	morse_encode(code, c);
	fprintf(logfile, "sending char %c code %s\n", c, code);
	i = 0;
	do {
		next = code[i];
		if(sig_sleep(10, 0) == -1){
			fprintf(logfile, "child timed out\n");
			exit(-1);
		}
		if (next == '.') kill(pid, MORSE_SHORT);
		else if (next == '-') kill(pid, MORSE_LONG);
		else if (next == '\0') kill(pid, MORSE_PAUSE);
		else kill(pid, MORSE_ERR);
		i++;
	} while(next != '\0');
	return 0;
}

char
get_char(int pid)
{
	int 	i;
	char 	code[8];

	i = 0;
	do {
		if (i == 0) sig_sleep(1, 0);
		kill(pid, sig_flag);
		if(sig_sleep(10, 0) == -1) {
			fprintf(logfile, "parent timed out\n");
			exit(-1);
		}
		fprintf(logfile, "sig_flag is now %s\n", strsignal(sig_flag));
		code[i] = sig_to_morse(sig_flag);
		i++;
	} while (sig_flag != MORSE_PAUSE); 
	fprintf(logfile, "code to decode: %s, decoded: %c\n", code, morse_decode(code));
	fprintf(logfile, "foobar\n");
	return morse_decode(code);
}
