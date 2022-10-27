#define MAX_ID_LENGTH   16
#define MAX_CANTONS     100

#define CANT_REMOVE_RETURN  "XXX000"

typedef char T_id[MAX_ID_LENGTH + 1];
typedef int T_canton;

typedef struct trainData {
    T_id id;
    T_canton canton;
    T_canton eoa;
} T_trainData;

typedef T_trainData T_trainsCollection[MAX_CANTONS+1];

typedef struct trainsNetwork {
    T_trainsCollection trains;
    int last; 
} T_trainsNetwork;


void trainsNetworkInit(T_trainsNetwork * trainNetwork);
void trainsNetworkShow(T_trainsNetwork * network);
T_canton trainsNetworkAdd(T_trainsNetwork * network, T_trainData * data);
int trainsNetworkSearch(T_trainsNetwork * network, T_id id);
int trainsNetworkCheckBeforeAdd(T_trainsNetwork * network, T_trainData * data);
T_canton trainsNetworkEdit(T_trainsNetwork * network, T_trainData * data, int index);
char * trainsNetworkDelete(T_trainsNetwork * network, T_id id, int index);
