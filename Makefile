DEBUG=0
ifeq ($(DEBUG), 1)
	CFLAGS = -Wall -Wextra -DDEBUG -g3
else
	CFLAGS = -Wall -Wextra -DNDEBUG 
endif

CC = gcc $(CFLAGS)

.PHONY: clean

base64_d: base64_d.o base64_d_cli.c
	$(CC) -o base64_d base64_d.o base64_d_cli.c

base64_d.o: base64_d.c
	$(CC) -c base64_d.c

randomart: randomart.c strtol_wrap.o base64_d.o
	$(CC) -o randomart randomart.c strtol_wrap.o base64_d.o

strtol_wrap: strtol_wrap_cli.c strtol_wrap.o
	$(CC) -o strtol_wrap strtol_wrap_cli.c strtol_wrap.o

strtol_wrap.o: strtol_wrap.c
	$(CC) -c strtol_wrap.c
clean:
	rm -f randomart base64_d strtol_wrap *.o
	rm -rf *.dSYM
