CC=gcc
.PHONY: all
all:client server

server: server.c
	gcc server.c -o server
	$(CC) -o $@ $^
	./server
		
client: client.c
	gcc client.c -o client
	$(CC) -o $@ $^
	./client

clean:
	rm -f server client