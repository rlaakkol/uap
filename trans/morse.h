
#define WAIT 3
#define MORSE_SHORT SIGUSR1
#define MORSE_LONG SIGUSR2
#define MORSE_PAUSE SIGINT
#define MORSE_ERR 'E'
#define INIT_FLAG '1'

#define MORSE_NCHARS 47
#define MORSE_LEN 8
#define MORSE_CODES {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----.", "-----", ".-.-.-", "..--..", "-....-", "---...", "-.-.-.", "-..-.", "..--.-", "......", ".......", "........"}
#define MORSE_MAP {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', ' ', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '.', '?', '-', ':', ';', '/', '_', '\n', EOF, '#'}


int morse_encode(char *buf, char c);

char morse_decode(char *code);

int sig_char(int pid, char *code);

char sig_to_morse(int sig);

int sig_sleep(void);
