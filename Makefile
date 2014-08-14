CFLAGS= -Wall -g

.PHONY : again clean clear_screen

all: randomart

clean:
	rm -f randomart

clear_screen:
	clear

again: clean clear_screen all
