all: server client

server: tcp_server.c
	gcc tcp_server.c -o server

client: tcp_client.c
	gcc tcp_client.c -o client

clean:
	rm -f server client uploaded.txt
