#include <stdio.h>
#include <unistd.h>
#include <signal.h>

static char 	codes[47][8] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----.", "-----", ".-.-.-", "..--..", "-....-", "---...", "-.-.-.", "-..-.", "..--.-", "......", ".......", "........"};
static char 	char_map[47] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', ' ', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '.', '?', '-', ':', ';', '/', '_', '\n', EOF, '#'};

int	sig_flag;


int
main(int argc, char** argv)
{
	int 	pid;
	FILE 	*infile, *outfile;
	
	sig_flag = INIT_FLAG;

	if ((pid = fork()) < 0){
		perror("fork error");
		exit(-1);
	}
	if (pid == 0) {
		// In child
		child_func(filename,infile);
	}
	else {
		// In parent
		parent_func(outfile, pid);
	}
	close(infile);
	close(outfile);
	return 0;
}

int
child_func(char *filename, FILE* infile)
{
	int 	ppid;
	char 	c, code[8];

	if (infile = fopen(filename) < 0) {
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
			sig_sleep()
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
		fputc(outfile, nextc);
		sig_flag = INIT_FLAG;
		kill(cpid, MORSE_PAUSE);
	}


}

char
sig_to_morse(int sig)
{
	if (sig == MORSE_SHORT) return '.';
	else if (sig == MORSE_LONG) return '-';
	else if (sig == MORSE_PAUSE) return '\0';
	else {
		exit(ERROR);
	}

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

int
sig_char(int pid, char code)
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
		else if (signo == MORSE_EOC) sig_flag = MORSE_EOC;
		else sig_flag = MORSE_ERR;
	}
}
		
