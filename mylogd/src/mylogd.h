#define FIFOPATH "/tmp/mylog.main"                                   
#define FIFOFMT "/tmp/mylog.%d" 
#define FIFOPERMS (S_IRWXU | S_IWGRP| S_IWOTH)                       
#define MAXLEN_OPEN 20
#define MAXLEN 256                                                   
#define TIMEOUT 60
#define CLOSE_MSG "C"



extern FILE *logfile;

int get_msg(int fd, char *buf, char delim);

int start_log(int pid);
