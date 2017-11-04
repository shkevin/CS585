#include "game.h"
#include <time.h>

int uniform_distribution(int rangeLow, int rangeHigh);
int randInRange(int min, int max);
void write_server_int(int sockfd, int msg);

int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    Move bestMove;
    bool first = true;
    char* serverReply = malloc(size*size*sizeof(char));
    char* message = malloc(size*size*sizeof(char));
    char** board = initializeBoard();
    srand((unsigned)time(NULL));

    player = 'X';
    opponent = 'O';

    //bestMove = findBestMove(board);
    // printf("Row: %d, Col: %d\n", bestMove.row, bestMove.col);

    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }

    puts("Connected\n");

    //keep communicating with server
    while(1)
    {
        // printf("Enter message : ");
        // scanf("%s" , message);

        //process first move here

        //Send some data
        if (!isMovesLeft(board))
        {
            printf("Tie");
            break;
        }

        if (first)
        {
            printf("%s\n", "here");
            bestMove.row = uniform_distribution(0,size-1);
            bestMove.col = uniform_distribution(0,size-1);
            printf("%s\n", "here");
            first = false;
        }
        else 
        {
            
            bestMove = findBestMove(board);
            board[bestMove.row][bestMove.col] = player;
            printf("%s\n", "above");
            message = sendBoard(board);
            printf("%s\n", "below");
        }

        if( send(sock, message, maxChar , 0) < 0)
        {
            //process first move here
            puts("Send failed");
            return 1;
        }

        puts(message);

        //Receive a reply from the server
        if(recv(sock , serverReply , maxChar , 0) < 0)
        {
            puts("Receive failed");
            break;
        }

        board = swapBoard(serverReply, board);
        puts(serverReply);
    }

    close(sock);
    free(board);
    free(message);
    free(serverReply);
    return 0;
}
/*
from https://stackoverflow.com/questions/11641629/generating-a-
     uniform-distribution-of-integers-in-c
*/
int uniform_distribution(int rangeLow, int rangeHigh) 
{
    double myRand = rand()/(1.0 + RAND_MAX); 
    int range = rangeHigh - rangeLow + 1;
    int myRand_scaled = (myRand * range) + rangeLow;
    return myRand_scaled;
}
