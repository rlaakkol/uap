#include "morse.h"
#include <signal.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>


const char codes[MORSE_NCHARS][MORSE_LEN] = MORSE_CODES;
const char char_map[MORSE_NCHARS] = MORSE_MAP;

FILE 	*logfile;

int
sig_sleep(time_t sec, long nsec)
{
	sigset_t 	sigs;
	struct timespec	sleeptime;

	/* Setup signals to wait for */
	sigemptyset(&sigs);
	sigaddset(&sigs, MORSE_SHORT);
	sigaddset(&sigs, MORSE_LONG);
	sigaddset(&sigs, MORSE_PAUSE);

	/* Define timeout */
	sleeptime.tv_sec = sec;
	sleeptime.tv_nsec = nsec;

	return sigtimedwait(&sigs, NULL, &sleeptime);
}


int
morse_encode(char *buf, char c)
{
	char 	cc;
	int 	i;
	
	/* Convert to lowercase */
	cc = tolower(c);
	/* Stupid-search for char in char_map */
	i = 0;
	while (i < 46 && char_map[i] != cc) {
		i++;
	}
	
	/* Copy code representing c to buf */
	strcpy(buf, codes[i]);
	return 1;
}

char
morse_decode(char *code)
{
	int i;

	/* Stupid-search for matching code in codes */
	i = 0;
	while (i < 47 && strcmp(code, codes[i])) {
		i++;
	}

	/* Return corresponding char */
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
	/* Loop throughout the morse-coded char */
	do {
		next = code[i];
		/* Wait for "parent ready" signal */
		if (sig_sleep(WAIT, 0) == -1) {
			/* Had to wait too long, timeout */
			fprintf(logfile, "%ld: child timed out (no signal received from parent)\n", (long int)time(NULL));
			fflush(logfile);
			exit(-1);
		}
		/* Parent ready, send correct signal */
		if (next == '.') kill(pid, MORSE_SHORT);
		else if (next == '-') kill(pid, MORSE_LONG);
		else kill(pid, MORSE_PAUSE);
		i++;
	} while (next == '-' || next == '.');
	return 0;
}

char
get_char(int pid)
{
	int 	i, signo;
	char 	code[8], c;
	
	signo = MORSE_PAUSE;
	i = 0;
	/* Loop until \0 is received */
	do {
		/* Signal "parent ready" to child with previous signal */
		kill(pid, signo);
		/* Wait for signal (if necessary) */
		if ((signo = sig_sleep(WAIT, 0)) == -1) {
			/* timeout */
			fprintf(logfile, "%ld: parent timed out (no signal received from child)\n", (long int)time(NULL));
			fflush(logfile);
			exit(-1);
		}
		/* Convert signal to (-, . or \0) */
		code[i] = sig_to_morse(signo);
		i++;
	} while (signo != MORSE_PAUSE); 
	/* Decode to char */
	c = morse_decode(code);
	return c;
}
