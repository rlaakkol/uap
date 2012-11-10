#include "morse.h"
#include <unistd.h>
#include <signal.h>
#include <time.h>


static char codes[47][8] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----.", "-----", ".-.-.-", "..--..", "-....-", "---...", "-.-.-.", "-..-.", "..--.-", "......", ".......", "........"};
static char char_map[47] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', ' ', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '.', '?', '-', ':', ';', '/', '_', '\n', EOF, '#'};

int
sig_sleep(void)
{
	struct timespec	sleeptime;

	sleeptime.tv_sec = WAIT;
	sleeptime.tv_nsec = 0;
	if (nanosleep(&sleeptime, NULL) != -1) {
		exit(ERROR);
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
	char 	c;
	int i;
	
	i = 0;
	while (i < 47 && !strcmp(test, codes[i])) {
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
		exit(ERROR);
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
}
