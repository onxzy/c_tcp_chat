#include "network.h"
#include "messagerie.h"

#include <stdio.h>
#include <sys/types.h>         
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define ACTION_EDIT_ID  1
#define ACTION_SHOW     2
#define ACTION_LEAVE    9

void main(int argc, char * const * argv) {
    if (argc != 4) {
        printf("Usage : %s server_ip server_port train_id\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int serverPort = atoi(argv[2]);
    char * serverIp = argv[1];
    char trainId[MAX_ID_LENGTH + 1];
    strncpy(trainId, argv[3], MAX_ID_LENGTH);
    trainId[MAX_ID_LENGTH + 1] = '\0';

    int mainSocket;
    struct sockaddr_in serverAddr, clientAddr;

    // Création du socket
    mainSocket = socket(AF_INET, SOCK_STREAM, 0);
    CHECK_ERR("Unable to open socket", mainSocket, -1);

    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = 0;
    clientAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // INADDR_ANY;
    int clientAddrLength = sizeof(clientAddr);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    serverAddr.sin_addr.s_addr = inet_addr(serverIp);

    printf("Server settings : %s:%d\n", serverIp, ntohs(serverAddr.sin_port));

    // Connexion au socket
    CHECK_ERR("Unable to connect to socket", bind(mainSocket, (const struct sockaddr * ) &clientAddr, sizeof(clientAddr)), -1);
    CHECK_ERR("Unable to get socket name", getsockname(mainSocket, (struct sockaddr *) &clientAddr, &clientAddrLength), -1);
    printf("Bind to socket %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

    CHECK_ERR("Unable to connect to server", connect(mainSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)), -1);

    // char recptBuffer[MAXOCTETS+1]; // buffer de reception
    // int lengthRecept ; // Nb octets recus

    T_trainData train = {"", 0, 0};
    strcpy(train.id, trainId);

    printf("[EVC Connected]\n");
    printf("\n");
    printf("%d Changer l'identifiant du train\n", ACTION_EDIT_ID);
    printf("%d Afficher les données du train\n", ACTION_SHOW);
    printf("%d Quitter\n", ACTION_LEAVE);
    printf("\n");
    printf("%d Rejoindre le réseau\n", REQ_ADDTRAIN);
    printf("%d Quitter le réseau\n", REQ_DELTRAIN);
    printf("%d Envoyer position\n", REQ_MVTRAIN);
    printf("%d Demande d'autorisation de mouvement\n", REQ_ASKMOVE);
    printf("\n");

    int action = 0;
    char actionBuffer[4];
    T_IndusMsg * resMsg = (T_IndusMsg *) malloc(sizeof(T_IndusMsg));

    while (1) {  

        printf("> ");
        fgets(actionBuffer, MAXOCTETS, stdin);
        action = atoi(actionBuffer);

        T_IndusMsg reqMsg = {0, 0, train}; 
        T_canton newLocation;

        switch (action) {
            case ACTION_EDIT_ID:
                printf("? WIP\n");
                break;

            case ACTION_SHOW:
                printf("[%s] (%d|%d)\n", train.id, train.canton, train.eoa);
                break;

            case ACTION_LEAVE:
                close(mainSocket);
                exit(EXIT_SUCCESS);
                printf("! Left\n");
                break;

            case REQ_ADDTRAIN:
                reqMsg.type = REQ_ADDTRAIN;
                write(mainSocket, &reqMsg, sizeof(T_IndusMsg));
                printf("...\n");

                read(mainSocket, resMsg, sizeof(T_IndusMsg));
                handle_client(&train, *resMsg);
                break;

            case REQ_DELTRAIN:
                reqMsg.type = REQ_DELTRAIN;
                write(mainSocket, &reqMsg, sizeof(T_IndusMsg));
                printf("...\n");

                read(mainSocket, resMsg, sizeof(T_IndusMsg));
                handle_client(&train, *resMsg);
                break;

            case REQ_MVTRAIN:
                printf("Nouvelle position (1-%d): ", train.eoa);
                scanf("%d",&newLocation);
                getchar();
                if (newLocation < 1) {
                    printf("! Nouvelle position invalide (<1)\n");
                    break;
                }


                reqMsg.type = REQ_MVTRAIN;
                reqMsg.data.canton = newLocation;
                write(mainSocket, &reqMsg, sizeof(T_IndusMsg));
                printf("...\n");

                read(mainSocket, resMsg, sizeof(T_IndusMsg));
                handle_client(&train, *resMsg);
                break;

            case REQ_ASKMOVE:
                printf("Position demandée (1-%d): ", train.eoa);
                scanf("%d",&newLocation);
                getchar();
                if (newLocation < 1) {
                    printf("! Position invalide (<1)\n");
                    break;
                }


                reqMsg.type = REQ_ASKMOVE;
                reqMsg.data.canton = newLocation;
                write(mainSocket, &reqMsg, sizeof(T_IndusMsg));
                printf("...\n");

                read(mainSocket, resMsg, sizeof(T_IndusMsg));
                handle_client(&train, *resMsg);
                break;
            
            default:
                printf("! Action inconnue\n");
                break;
        }
    }
}

