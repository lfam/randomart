debug=1
GCC_FLAGS = -Wall -Wextra -DDEBUG -g3
CLANG_FLAGS = -Weverything -DDEBUG -g -O0
ifeq ($(debug), 0)
	GCC_FLAGS = -Wall -Wextra -DNDEBUG 
	CLANG_FLAGS = -Weverything -DNDEBUG
endif

GCC = gcc $(GCC_FLAGS)
CLANG = clang $(CLANG_FLAGS)

COMPILER = $(GCC)
ifeq ($(compiler), clang)
	COMPILER = $(CLANG)
endif

CC = $(COMPILER)

.PHONY: clean

randomart: randomart.c strtol_wrap.o base64_d.o
	$(CC) -o randomart randomart.c strtol_wrap.o base64_d.o

base64_d: base64_d.o base64_d_cli.c
	$(CC) -o base64_d base64_d.o base64_d_cli.c

base64_d.o: base64_d.c
	$(CC) -c base64_d.c

strtol_wrap: strtol_wrap_cli.c strtol_wrap.o
	$(CC) -o strtol_wrap strtol_wrap_cli.c strtol_wrap.o

strtol_wrap.o: strtol_wrap.c
	$(CC) -c strtol_wrap.c
clean:
	rm -f randomart base64_d strtol_wrap *.o
	rm -rf *.dSYM
