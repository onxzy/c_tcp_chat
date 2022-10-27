#include "locationManager.h"

#include <stdlib.h>
#include <stdio.h>

void main(int argc, char const *argv[])
{
    T_trainsNetwork * network = (T_trainsNetwork *) malloc(sizeof(T_trainsNetwork));
    trainsNetworkInit(network);

    trainsNetworkShow(network);

    T_trainData train_1 = {"train_1", 0, 0};
    int train_1_index;
    trainsNetworkAdd(network, &train_1);

    train_1_index = trainsNetworkSearch(network, "train_1");
    train_1.canton = 1;
    trainsNetworkEdit(network, &train_1, train_1_index);

    train_1_index = trainsNetworkSearch(network, "train_1");
    train_1.canton = 3;
    trainsNetworkEdit(network, &train_1, train_1_index);

    trainsNetworkShow(network);

    T_trainData train_2 = {"train_2", 0, 0};
    int train_2_index;
    trainsNetworkAdd(network, &train_2);

    trainsNetworkShow(network);

    train_2_index = trainsNetworkSearch(network, "train_2");
    train_2.canton = 1;
    trainsNetworkEdit(network, &train_2, train_2_index);

    trainsNetworkShow(network);

    train_1_index = trainsNetworkSearch(network, "train_1");
    train_1.canton = 5;
    trainsNetworkEdit(network, &train_1, train_1_index);

    trainsNetworkShow(network);



    train_2_index = trainsNetworkSearch(network, "train_2");
    train_2.canton = 4;
    trainsNetworkEdit(network, &train_2, train_2_index);

    train_1_index = trainsNetworkSearch(network, "train_1");
    train_1.canton = 100;
    trainsNetworkEdit(network, &train_1, train_1_index);

    trainsNetworkShow(network);

    train_1_index = trainsNetworkSearch(network, "train_1");
    trainsNetworkDelete(network, train_1.id, train_1_index);

    trainsNetworkShow(network);

    train_2_index = trainsNetworkSearch(network, "train_2");
    train_2.canton = 100;
    trainsNetworkEdit(network, &train_2, train_2_index);

    train_2_index = trainsNetworkSearch(network, "train_2");
    trainsNetworkDelete(network, train_2.id, train_2_index);

    trainsNetworkShow(network);
}
