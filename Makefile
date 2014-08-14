CFLAGS= -Wall -g

.PHONY : again all clean

all: randomart io

clean:
	rm -f randomart io

again: clean all
