.PHONY: clean

CFLAGS := -std=c2x -g -Wall -Wextra -Wno-psabi -O2 -fsanitize=address,undefined -fno-omit-frame-pointer

CC := gcc
#CC := clang

all:
	$(CC) $(CFLAGS) -c -o fmt.o fmt.c
	$(CC) $(CFLAGS) -c -o main.o main.c
	$(CC) $(CFLAGS) -o main.bin fmt.o main.o

clean:
	rm -f *.o
	rm main.bin

