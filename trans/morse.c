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
volatile int 	ready_flag;
FILE 	*logfile;


int
sig_sleep(time_t sec, long nsec)
{
	sigset_t 	sigs;
	struct timespec	sleeptime;

	sigemptyset(&sigs);
	sigaddset(&sigs, MORSE_SHORT);
	sigaddset(&sigs, MORSE_LONG);
	sigaddset(&sigs, MORSE_PAUSE);

	sleeptime.tv_sec = sec;
	sleeptime.tv_nsec = nsec;

	return sigtimedwait(&sigs, NULL, &sleeptime);
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
	while (i < 47 && strcmp(code, codes[i])) {
		i++;
	}

	
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
	i = 0;
	do {
		next = code[i];
		sig_sleep(100, 0);
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
	int 	i, signo;
	char 	code[8], c;
	
	signo = MORSE_PAUSE;
	i = 0;
	do {
		kill(pid, signo);
		signo = sig_sleep(1, 0);
		code[i] = sig_to_morse(signo);
		i++;
	} while (signo != MORSE_PAUSE); 
	c = morse_decode(code);
	return c;
}
