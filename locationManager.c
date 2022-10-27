#include "locationManager.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void trainsNetworkInit(T_trainsNetwork * network) {
    // network = (T_trainsNetwork *) malloc(sizeof(T_trainsNetwork));

    assert(network != NULL);

    network->last = -1;

    return;
}

void trainsNetworkShow(T_trainsNetwork * network) {
    assert(network != NULL);

    if (network->last < 1) return (void) printf("Empty\n");

    printf("Size : %d\n", network->last);
    printf("               id | canton | eoa \n");
    for (int i = 1; i <= network->last; i++)
    {
        T_trainData train = network->trains[i];
        printf(" %16s | %6d | %3d \n", train.id, train.canton, train.eoa);
    }  
}

T_trainData getLast(T_trainsNetwork * network) {
    assert(network != NULL);
    assert(network->last >= 0);
    return (network->trains[network->last]);
}

T_canton trainsNetworkAdd(T_trainsNetwork * network, T_trainData * data) {
    assert(network != NULL);
    
    if (network->last >= MAX_CANTONS) return -1;
    if (trainsNetworkSearch(network, data->id) != -1) return -1;

    if (network->last < 1) {
        data->canton = 1;
        data->eoa = MAX_CANTONS;
        network->trains[1] = *data;
        network->last = 1;
        return MAX_CANTONS;
    }
    if (getLast(network).canton == 1) return -1;
    data->canton = 1;
    data->eoa = getLast(network).canton - 1;
    network->trains[network->last + 1] = *data;
    network->last++; 
    return data->eoa;
}

int trainsNetworkCheckBeforeAdd(T_trainsNetwork * network, T_trainData * data) {
    assert(network != NULL);

    if (network->last >= MAX_CANTONS) return 0;
    if (trainsNetworkSearch(network, data->id) == -1) return 0;
    if (network->last < 1) return 1;
    if (getLast(network).canton == 1) return 0;
    return 1;
}

int trainsNetworkSearch(T_trainsNetwork * network, T_id id) {
    assert(network != NULL);
    if (network->last < 1) return -1;
    for (int i = 1; i <= network->last; i++) {
        T_trainData train = network->trains[i];
        if (strcmp(id, train.id) == 0) return i;
    } 
    return -1;
}

T_canton trainsNetworkEdit(T_trainsNetwork * network, T_trainData * data, int index) {
    assert(network != NULL);
    if (network->last < index) return -1;
    // if (strcmp(network->trains[index].id, data->id) != 0) return -1;

    if (data->canton > network->trains[index].eoa) return -1;

    network->trains[index].canton = data->canton;

    int lastCanton = data->canton;
    for (int i = index + 1; i <= network->last; i++)
    {
        T_trainData * train = &(network->trains[i]);
        train->eoa = lastCanton - 1;
        lastCanton = train->canton;
    }

    return network->trains[index].eoa;
}

char * trainsNetworkDelete(T_trainsNetwork * network, T_id id, int index) {
    assert(network != NULL);
    if (network->last < index) return CANT_REMOVE_RETURN;
    if (network->trains[index].canton != MAX_CANTONS) return CANT_REMOVE_RETURN;

    if (network->last >= index + 1) {
        network->trains[index+1].eoa = MAX_CANTONS;
    }

    for (int i = index; i < network->last; i++)
    {
        network->trains[i] = network->trains[i+1];
    }

    network->last--;
    return id;
    
}
