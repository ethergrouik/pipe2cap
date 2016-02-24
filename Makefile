CC=gcc
CFLAGS=-Wall -Werror -g -O2
LDFLAGS=-lpcap

pipe2cap: main.c
	$(CC) main.c -o pipe2cap $(CFLAGS) $(LDFLAGS)

clean:
	rm pipe2cap
