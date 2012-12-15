CC = gcc 
CFLAGS = -std=gnu99 -Wall -Wextra -pedantic -g
DSOURCES = src/mylogd.c src/mylogd_logger.c
TSOURCES = tester.c
LSOURCES = src/mylog.c
LHEADERS = src/mylog.h
DOBJS = $(DSOURCES:.c=.o)
TOBJS = $(TSOURCES:.c=.o)
LOBJS = $(LSOURCES:.c=.o)
OBJS = $(DOBJS) $(TOBJS) $(LOBJS)
LIB = lib/mylog.a
DAEMON = daemon/mylogd
TESTER = tester

all: $(DAEMON) $(LIB) $(TESTER) clean

$(DAEMON): $(DOBJS)
	$(CC) -pthread $(DOBJS) -o $@

$(LIB): $(LOBJS)
	ar rvs $(LIB) $(LOBJS)

$(TESTER): $(LIB) $(TOBJS)


$(LOBJS): $(LHEADERS)

clean:
	rm -f $(OBJS)
