#ifndef NETWORK_H
#define NETWORK_H

typedef struct Network Network;
typedef struct Layer Layer;
typedef struct Neuron Neuron;
typedef struct Vector Vector;

typedef enum LayerType {INPUT, HIDDEN, OUTPUT} LayerType;
typedef enum ActFctType {SIGMOID, TANH} ActFctType;

struct Vector{
    int size;
    double vals[];
};

struct Neuron{
    double bias;
    double actv;
    int nbWeights;
    double weights[];
};

struct Layer{
    int ncount;
    Neuron neurons[];
};

struct Network{
    int inpNodeSize;
    int inpLayerSize;
    int hidNodeSize;
    int hidLayerSize;
    int outNodeSize;
    int outLayerSize;
    double learningRate;
    ActFctType hidLayerActType;
    ActFctType outLayerActType;
    Layer layers[];
};

Network *createNetwork(int inpCount, int hidCount, int outCount);
Neuron *getNeuron(Layer *l, int neuronId);
Layer *getLayer(Network *nn, LayerType ltype);
void feedInput(Network *nn, Vector *v);
void frontProp(Network *nn);
void backProp(Network *nn, int target);
int getResult(Network *nn);

#endif