#include "game.h"
#include <time.h>

/*
 inspired from https://github.com/mindcruzer/tic-tac-toe/blob/master/
*/

int uniform_distribution(int rangeLow, int rangeHigh);
int randInRange(int min, int max);
void write_server_int(int sockfd, int msg);

int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char* serverReply = malloc(size*size*sizeof(char));
    char* message = malloc(size*size*sizeof(char));
    char** board = initializeBoard();
    srand((unsigned)time(NULL));

    int firstRow = uniform_distribution(0,size-1);
    int firstCol = uniform_distribution(0,size-1);

    // sendBoard(board);
    printf("%d, %d\n", firstRow, firstCol);
    message = sendBoard(board);

    printf("%s\n", message);

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

        // puts("Server reply :");
        // puts(server_reply);
        // processBoard(server_reply);
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
