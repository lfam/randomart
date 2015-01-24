# www.icosaedro.it/c-modules.html
debug=1
GCC_FLAGS = -Wall -Wextra -DDEBUG -g3
CLANG_FLAGS = -Weverything -DDEBUG -g -O0
ifeq ($(debug), 0)
	GCC_FLAGS = -Wall -Wextra -DNDEBUG 
	CLANG_FLAGS = -Weverything -DNDEBUG
endif

GCC = gcc
CLANG = clang

COMPILER = $(GCC)
CFLAGS = $(GCC_FLAGS)
ifeq ($(compiler), clang)
	COMPILER = $(CLANG)
	CFLAGS = $(CLANG_FLAGS)
endif

CC = $(COMPILER)

.PHONY: clean

BIN = randomart
OBJS = strtol_wrap.o base64_d.o

$(BIN): $(OBJS) $(BIN).c
	$(CC) $(CFLAGS) $(OBJS) $(BIN).c -o $(BIN)

%.o: %.c %.h
	$(CC) -c $(CFLAGS) $< -o $@

base64_d: base64_d_cli.c base64_d.o 
	$(CC) $(CFLAGS) $@.o $< -o $@

strtol_wrap: strtol_wrap_cli.c strtol_wrap.o
	$(CC) $(CFLAGS) $@.o $< -o $@

clean:
	rm -f $(BIN) base64_d strtol_wrap *.o a.out
	rm -rf *.dSYM
