#include "game.h"

int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char* message = malloc(20*sizeof(char));
    char* reply = malloc(20*sizeof(char));
    char** board = initializeBoard();
    Move move;
    player = 'O';
    opponent = 'X';

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( port );

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc , 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted");

    //Receive a message from client
    while( (read_size = recv(client_sock , reply , maxChar , 0)) > 0 )
    {
        //Send the message back to client
        if (!isMovesLeft(board))
        {
        	board = setBoard(board);
        	write(client_sock, "tie", strlen("tie"));
        	continue;
        }

		board = swapBoard(reply, board);
        move = findBestMove(board, player);
	    board[move.row][move.col] = player;
        message = sendBoard(board);
        write(client_sock , message , maxChar);
    }

    if(read_size == 0)
    {
        puts("Client disconnected");
        free(board);
    	free(message);
    	free(reply);
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
    return 0;
}