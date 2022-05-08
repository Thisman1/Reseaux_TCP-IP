CC=gcc
.PHONY: all
all:client server

server: server.c
	gcc server.c -o server
	$(CC) -o $@ $^
	./server localhost
		
client: client.c
	gcc client.c -o client
	$(CC) -o $@ $^
	./client localhost

serverUDP: serverUDP.c
	gcc serverUDP.c -o serverUDP
	$(CC) -o $@ $^
	./serverUDP

clientUDP: clientUDP.c
	gcc clientUDP.c -o clientUDP
	$(CC) -o $@ $^
	./clientUDP

clean:
	rm -f server client serverUDP clientUDP