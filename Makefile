DEBUG=0
ifeq ($(DEBUG), 1)
	CFLAGS = -Wall -Wextra -DDEBUG -g3
else
	CFLAGS = -Wall -Wextra -DNDEBUG 
endif

CC = gcc $(CFLAGS)

.PHONY: clean

base64_d_cli: base64_d.o base64_d_cli.o
	$(CC) -o base64_d_cli base64_d.o base64_d_cli.o

base64_d_cli.o: base64_d_cli.c
	$(CC) -c base64_d_cli.c

base64_d.o: base64_d.c
	$(CC) -c base64_d.c

randomart: randomart.o strtol_wrap.o base64_d.o
	$(CC) -o randomart randomart.o strtol_wrap.o base64_d.o

randomart.o: randomart.c
	$(CC) -c randomart.c

strtol_wrap_cli: strtol_wrap_cli.o strtol_wrap.o
	$(CC) -o strtol_wrap_cli strtol_wrap_cli.o strtol_wrap.o

strtol_wrap_cli.o: strtol_wrap_cli.c
	$(CC) -c strtol_wrap_cli.c

strtol_wrap.o: strtol_wrap.c
	$(CC) -c strtol_wrap.c
clean:
	rm -f randomart base64_d_cli strtol_wrap_cli *.o
