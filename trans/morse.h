

#define MORSE_SHORT SIGUSR1
#define MORSE_LONG SIGUSR2
#define MORSE_PAUSE SIGINT
#define MORSE_ERR 'E'
#define INIT_FLAG '1'

extern char codes[][];
extern char char_map[];

int morse_encode(char *buf, char c);

char morse_decode(char *code);

int sig_char(int pid, char *code);

char sig_to_morse(int sig);

int sig_sleep(void);
