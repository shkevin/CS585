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
    char* serverReply = malloc(maxChar*sizeof(char));
    char* message = malloc(maxChar*sizeof(char));
    char* messageTo = malloc(maxChar*sizeof(char));
    char** board = initializeBoard();
    srand((unsigned)time(NULL));

    message = sendBoard(board);

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
        if (!isMovesLeft(board))
        {
            // printf("Tie");
            break;
        }

        if (first)
        {
            puts("Fresh board");
            puts(message);
            bestMove.row = uniform_distribution(0,size-1);
            bestMove.col = uniform_distribution(0,size-1);
            board[bestMove.row][bestMove.col] = player;
            first = false;
        }

        message = sendBoard(board);
        if( send(sock, message, maxChar , 0) < 0)
        {
            //process first move here
            puts("Send failed");
            return 1;
        }

        //Receive a reply from the server
        if(recv(sock , serverReply , maxChar , 0) < 0)
        {
            puts("Receive failed");
            break;
        }

        puts(serverReply);
        puts("swapping board from server");
        board = swapBoard(serverReply, board);
        bestMove = findBestMove(board, player);
        board[bestMove.row][bestMove.col] = player;
        message = sendBoard(board);
        puts("after client move");
        puts(message);
    }

    close(sock);
    free(board);
    free(message);
    free(serverReply);
    return 0;
}

