DEBUG=0
ifeq ($(DEBUG), 1)
	CFLAGS = -Wall -Wextra -DDEBUG -g3
else
	CFLAGS = -Wall -Wextra -DNDEBUG 
endif

CC = gcc $(CFLAGS)

base64_d:
	$(CC) base64_d.c libbase64_d.c -o base64_d

randomart:
	$(CC) randomart.c -o randomart

strtol_wrap:
	$(CC) strtol_wrap.c libstrtol_wrap.c -o strtol_wrap
clean:
	rm randomart base64_d strtol_wrap
