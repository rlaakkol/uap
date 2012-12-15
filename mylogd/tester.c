#include "src/mylog.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
	mylog_open("tester");
	sleep(atoi(argv[1]));
	printf("logging msg1\n");
	mylog_write("test 1 of log");
	sleep(atoi(argv[2]));
	printf("logging msg2\n");
	mylog_write("test 2 of log");
	mylog_close();
	return 0;
}
