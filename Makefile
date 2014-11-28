DEBUG ?= 1
ifeq (DEBUG, 1)
	CFLAGS =-g3
else
	CFLAGS =-DNDEBUG
endif

CC = gcc $(CFLAGS)

base64_d:
	gcc -Wall -Wextra -g base64_d.c libbase64_d.c -o base64_d

randomart:
	gcc -Wall -Wextra -g randomart.c -o randomart

clean:
	rm randomart base64_d
