#include "src/mylog.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
	char 	logname[12];

	if (argc != 4) return -1;
	sprintf(logname, "tester%d", atoi(argv[1]));
	mylog_open(logname);
	sleep(atoi(argv[2]));
	mylog_write("log test 1");
	sleep(atoi(argv[3]));
	mylog_write("Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! Too long! ");
	mylog_close();
	return 0;
}
