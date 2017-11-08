PROGRAMS=client server

all: $(PROGRAMS)

client: client.c
	gcc -o client client.c

server: server.c
	gcc -o server server.c

clean:
	rm -r client server