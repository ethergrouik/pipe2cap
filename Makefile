CC=gcc
LDFLAGS=-lpcap

pipe2cap: main.c
	$(CC) main.c -o pipe2cap $(LDFLAGS)

clean:
	rm pipe2cap
