#define CLOSE_MSG "C"
#define FIFOFMT "/tmp/mylog.%d"
#define FIFOPATH "/tmp/mylog.main"

// If you change this, remember to also change it from the daemon's settings
#define MAXLEN 256
#define START_MSG "Started logging."

/*
 * This function is used for opening the connection with the mylogd-daemon.
 * Remamber to have the daemon running before attempting to open connection.
 * The tag argument will ge displayed in each log message for this connection.
 * */
int mylog_open(const char *tag);

/* 
 * Use this function to write log messages. Just provide your message as the
 * msg argument. The total length of a log message is limited to 256
 * characters, which leaves about 200 characters for your message. Too long
 * messages will be trunkated.  */
int mylog_write(const char *msg);

/*
 * This function will close the existing log connection. Remember to call this
 * function before returning from your program. Otherwise the daemon won't know
 * when to release resources reserved for you.  */
int mylog_close(void);
