CC = gcc
CFLAGS = -Wall -Wextra

SERVER = server_mux
CLIENT = client

SRCS_SERVER = tcp_server_multiplex.c
SRCS_CLIENT = tcp_client.c

all: $(SERVER) $(CLIENT)

$(SERVER): $(SRCS_SERVER)
	$(CC) $(CFLAGS) $(SRCS_SERVER) -o $(SERVER)

$(CLIENT): $(SRCS_CLIENT)
	$(CC) $(CFLAGS) $(SRCS_CLIENT) -o $(CLIENT)

clean:
	rm -f $(SERVER) $(CLIENT) uploaded.txt
