#include "messagerie.h"

#include <string.h>
#include <stdio.h>

int checkReq(T_IndusMsg req) {
    return 1;
}

T_IndusMsg handle_server(T_trainsNetwork * network, T_IndusMsg req) {
    T_trainData train = req.data;
    T_IndusMsg res = {1, 0, {"", 0, 0}};
    
    int index;
    T_canton eoa;
    char * id;
    switch (req.type) {
        case REQ_ADDTRAIN:
            // T_trainData * newTrain = (T_trainData *) malloc(sizeof(T_trainData));
            // *newTrain = req.data;
            if (trainsNetworkAdd(network, &train) != -1) {
                res.type = RES_ADDTRAIN;
                res.data = train;
            } else {
                res.type = ERR_ADDTRAIN;
            }
            break;

        case REQ_MVTRAIN:
            index = trainsNetworkSearch(network, req.data.id);
            if (index == -1) {
                res.type = ERR_MVTRAIN;
                break;
            }

            eoa = trainsNetworkEdit(network, &train, index);
            if (eoa != -1) {
                res.type = RES_MVTRAIN;
                res.data = train;
            } else {
                res.type = ERR_MVTRAIN;
            }
            break;

        case REQ_ASKMOVE:
            if (trainsNetworkCheckBeforeAdd(network, &train)) {
                res.type = RES_ASKMOVE;
            } else {
                res.type = ERR_ASKMOVE;
            }
            break;

        case REQ_DELTRAIN:
            index = trainsNetworkSearch(network, req.data.id);
            if (index == -1) {
                res.type = ERR_DELTRAIN;
                break;
            }

            id = trainsNetworkDelete(network, req.data.id, index);
            if (strcmp(id, CANT_REMOVE_RETURN) != 0) {
                res.type = RES_DELTRAIN;
            } else {
                res.type = ERR_DELTRAIN;
            }
            break;
    }

    return res;
} 

void handle_client(T_trainData * train, T_IndusMsg res) {
    switch (res.type) {
        case RES_ADDTRAIN:
            printf("# Train joined network\n");
            train->canton = res.data.canton;
            train->eoa = res.data.eoa;
            break;

        case ERR_ADDTRAIN:
            printf("# Can't join network\n");
            break;

        case RES_ASKMOVE:
            printf("# Movement authorized\n");
            break;

        case ERR_ASKMOVE:
            printf("# Movement not authorized\n");
            break;

        case RES_MVTRAIN:
            printf("# Location refreshed\n");
            train->canton = res.data.canton;
            train->eoa = res.data.eoa;
            break;

        case ERR_MVTRAIN:
            printf("# Error refreshing location\n");
            break;
        
        case RES_DELTRAIN:
            printf("# Train left network\n");
            break;

        case ERR_DELTRAIN:
            printf("# Can't leave network\n");
            break;
    
        default:
            printf("# Server error\n");
            break;
    }
}

