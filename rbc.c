#include "network.h"
#include "rbc.h"

#include <stdio.h>
#include <sys/types.h>         
#include <sys/socket.h>
#include <sys/prctl.h>
#include <signal.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

int max(int a, int b) {
    if(a < b) return b;
    else return a;
}

void main() {
    int listenSocket;
    int mainSocket;

    struct sockaddr_in serverAddr, clientAddr;
    int lengthAddr ; //Indique la longeur d'une adresse

    int fork_pid;
    
    char recptBuffer[MAXOCTETS+1]; //buffer de reception
    T_IndusMsg * reqMsg = (T_IndusMsg *) malloc(sizeof(T_IndusMsg));

    char sendBuffer[MAXOCTETS+1]; //buffer d'émission
    int lengthRecept ; // Nb octets recus

    // Init Socket
    listenSocket = socket(AF_INET,SOCK_STREAM,0);
    CHECK_ERR("Unable to open listen socket", listenSocket, -1);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // INADDR_ANY;

    CHECK_ERR("Unable to connect to socket", bind(listenSocket, (const struct sockaddr *) &serverAddr, sizeof(serverAddr)), -1);

    // Init Pipe
    int father_pipe[2];
    CHECK_ERR("Unable to create father pipe", pipe(father_pipe), -1);

    T_pipeExchange * father_r_data = (T_pipeExchange *) malloc(sizeof(T_pipeExchange));
    T_pipeExchange * son_r_data = (T_pipeExchange *) malloc(sizeof(T_pipeExchange));

    // Init Select
    fd_set rfds;
    int fd_val;

    // Init Network
    T_trainsNetwork * network = (T_trainsNetwork *) malloc(sizeof(T_trainsNetwork));
    trainsNetworkInit(network);

    // Listen
    listen(listenSocket, 5);
    printf("Listening on %s:%d\n", inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port));

    // Main Loop
    while (1) {
        FD_ZERO(&rfds);
        FD_SET(listenSocket, &rfds);
        FD_SET(father_pipe[0], &rfds);
  
        fd_val = select(max(listenSocket, father_pipe[0]) + 1, &rfds, NULL, NULL, NULL);
        if (fd_val == -1) {
            printf("error select 1\n");
            exit(EXIT_FAILURE);
        }

        if (FD_ISSET(listenSocket, &rfds)) {   
            mainSocket = accept(listenSocket, NULL, NULL);
            CHECK_ERR("Unable to open main socket", mainSocket, -1);

            int son_pipe[2];
            CHECK_ERR("Unable to create son pipe", pipe(son_pipe), -1);

            if (fork_pid = fork()) { // Father

                close(mainSocket); //On ferme la socket de dialogue du fils

            } else { // Son

                prctl(PR_SET_PDEATHSIG, SIGTERM);
                close(listenSocket);

                while (1) {
                    read(mainSocket, reqMsg, sizeof(T_IndusMsg));

                    if (reqMsg->type != 0) {
                        printf("%d\n", reqMsg->type);
                        
                        T_pipeExchange w_data = {son_pipe[1], *reqMsg};
                        // strcpy(w_data.msg, recptBuffer);
                        write(father_pipe[1], &w_data, sizeof(T_pipeExchange));

                        reqMsg->type = 0;

                        read(son_pipe[0], son_r_data, sizeof(T_pipeExchange));
                        write(mainSocket, &(son_r_data->msg), sizeof(T_IndusMsg));
                    }
                }
            }
        } else if (FD_ISSET(father_pipe[0], &rfds)) {
            read(father_pipe[0], father_r_data, sizeof(T_pipeExchange));

            printf("[%d] %d : ", father_r_data->r, father_r_data->msg.type);

            T_IndusMsg resMsg = handle_server(network, father_r_data->msg);
            printf("%d\n", resMsg.type);

            trainsNetworkShow(network);

            T_pipeExchange w_data = {father_pipe[1], resMsg};
            write(father_r_data->r, &w_data, sizeof(T_pipeExchange));
        } else {
            printf("error select 2\n");
            exit(EXIT_FAILURE);
        }            
    }

    close(listenSocket);
    return;    
    
}
