DEBUG=0
ifeq ($(DEBUG), 1)
	CFLAGS = -Wall -Wextra -DDEBUG -g3
else
	CFLAGS = -Wall -Wextra -DNDEBUG 
endif

CC = gcc $(CFLAGS)

.PHONY: clean

base64_d_cli:
	$(CC) base64_d_cli.c base64_d.c -o base64_d_cli

randomart: randomart.o strtol_wrap.o
	$(CC) -o randomart randomart.o strtol_wrap.o

randomart.o: randomart.c
	$(CC) -c randomart.c

strtol_wrap.o: strtol_wrap.c
	$(CC) -c strtol_wrap.c
clean:
	rm -f randomart base64_d strtol_wrap *.o
