#ifndef _MORSE_H_
#define _MORSE_H_

#define WAIT 10	/* Timeout value in seconds */

/* Signal definitions */
#define MORSE_SHORT SIGUSR1
#define MORSE_LONG SIGUSR2
#define MORSE_PAUSE SIGINT

/* Morse-letter -mappings, "dictionary" */
#define MORSE_NCHARS 47
#define MORSE_LEN 8
#define MORSE_CODES {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----.", "-----", ".-.-.-", "..--..", "-....-", "---...", "-.-.-.", "-..-.", "..--.-", "......", ".......", "........"}
#define MORSE_MAP {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', ' ', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '.', '?', '-', ':', ',', '/', '_', '\n', EOF, '#'}

extern FILE *logfile;

/* The actual functions used by the main program
 * #############################################
 */

/* Signal character c to process pid using morse. */
int sig_char(int pid, char c);

/* "ping" process pid for one character. */
char get_char(int pid);


/* Helper functions
 * #############################################
 */

/* Encode char c into morse-code. Store result into buf.
 * Maximum length of the code is 8 chars, including \0. */
int morse_encode(char *buf, char c);

/* Decode morse-coded character. Read code from code
 * and return result. */
char morse_decode(char *code);

/* Convert signo (into) into morse code char (-, . or \0) */
char sig_to_morse(int sig);

/* If there are no blocked signals pending, sleep until
 * signal arrives or timeout (sec seconds and nsec nanoseconds)
 * occurs */
int sig_sleep(time_t sec, long nsec);

#endif
