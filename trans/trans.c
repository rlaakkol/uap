#include <stdio.h>
#include <unistd.h>
#include <signal.h>

static char 	codes[47][8] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----.", "-----", ".-.-.-", "..--..", "-....-", "---...", "-.-.-.", "-..-.", "..--.-", "......", ".......", "........"};
static char 	char_map[47] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', ' ', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '.', '?', '-', ':', ';', '/', '_', '\n', EOF, '#'};



int
main(int argc, char** argv)
{
	int 	pid;
	FILE 	*infile, *outfile;

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
	
	while () {

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
