#include "morse.h"
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


const char codes[MORSE_NCHARS][MORSE_LEN] = MORSE_CODES;
const char char_map[MORSE_NCHARS] = MORSE_MAP;

int
sig_sleep(void)
{
	struct timespec	sleeptime;

	sleeptime.tv_sec = WAIT;
	sleeptime.tv_nsec = 0;
	if (nanosleep(&sleeptime, NULL) != -1) {
		exit(-1);
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
sig_char(int pid, char *code)
{
	char	c;
	int	i;
	i = 0;
	while((c = code[i]) != '\0') {
		if (i > 0) {
			sig_sleep();
		}
		if (c == '.') kill(pid, MORSE_SHORT);
		else if (c == '-') kill(pid, MORSE_LONG);
		else kill(pid, MORSE_ERR);
		i++;
	}
	return 0;
}
