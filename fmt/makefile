.PHONY: clean

CFLAGS := -std=c2x -g -Wall -Wextra -Wno-psabi -O2 -fsanitize=address,undefined -fno-omit-frame-pointer

all:
	gcc $(CFLAGS) -c -o fmt.o fmt.c
	gcc $(CFLAGS) -c -o main.o main.c
	gcc $(CFLAGS) -o main.bin fmt.o main.o

clean:
	rm -f *.o
	rm main.bin

