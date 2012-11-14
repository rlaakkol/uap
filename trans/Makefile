CC = gcc 

CFLAGS = -Wall -Wextra -pedantic

SOURCES = src/trans.c src/morse.c

LIBS = src/morse.h

OBJS = $(SOURCES:.c=.o)

EXE = trans

all: $(EXE) clean

$(EXE): $(OBJS)
	$(CC) $(OBJS) -o $@

$(OBJS): $(LIBS)

clean:
	rm -f $(OBJS)
