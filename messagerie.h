#include "locationManager.h"

#define REQ_ADDTRAIN    101
#define RES_ADDTRAIN    201
#define ERR_ADDTRAIN    501

#define REQ_DELTRAIN    102
#define RES_DELTRAIN    202
#define ERR_DELTRAIN    502

#define REQ_MVTRAIN     103
#define RES_MVTRAIN     203
#define ERR_MVTRAIN     503

#define REQ_ASKMOVE     104
#define RES_ASKMOVE     204
#define ERR_ASKMOVE     504

typedef struct IndusMsg {
    int reqack;
    int type;
    T_trainData data;
} T_IndusMsg;

T_IndusMsg handle_server(T_trainsNetwork * network, T_IndusMsg req);
void handle_client(T_trainData * train, T_IndusMsg res);
