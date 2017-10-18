CC = gcc
CFLAGS = -Wall -Wextra -O3
LDFLAGS = -lwsock32

all: tcp-sock-client tcp-sock-server udp-sock-client udp-sock-server

tcp-sock-client: tcp-sock-client.c
	$(CC) $(CFLAGS) $+ -o $@ $(LDFLAGS)

tcp-sock-server: tcp-sock-server.c
	$(CC) $(CFLAGS) $+ -o $@ $(LDFLAGS)

udp-sock-client: udp-sock-client.c
	$(CC) $(CFLAGS) $+ -o $@ $(LDFLAGS)

udp-sock-server: udp-sock-server.c
	$(CC) $(CFLAGS) $+ -o $@ $(LDFLAGS)

clean:
	del *.exe
