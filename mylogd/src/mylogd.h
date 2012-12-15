#define FIFOPATH "/tmp/mylog.main"                                   
#define FIFOFMT "/tmp/mylog.%d" 
#define FIFOPERMS (S_IRWXU | S_IWGRP| S_IWOTH)                       
#define MAXLEN 256                                                   
#define TIMEOUT 5



extern FILE *logfile;

int get_msg(int fd, char *buf, char delim);

int start_log(int pid);
