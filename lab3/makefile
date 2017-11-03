PROGRAMS=client server

all: $(PROGRAMS)

client: client.c
	gcc -o client client.c game.c

server: server.c
	gcc -o server server.c game.c

clean:
	rm -r client server