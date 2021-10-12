#ifndef NETWORK_H
#define NETWORK_H

#include "layer.h"

int initNetwork(void);
void feedInput(int p);
void forwardProp(void);
void calculateCosts(int p);
void backProp(int p);
void updateWeights(void);
void train(void);
void printData(void);
void test(void);

#endif
