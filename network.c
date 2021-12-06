#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "network.h"

// GET NEURON
Neuron *getNeuron(Layer *l, int neuronId) 
{
    int nodeSize = sizeof(Neuron) + (l->neurons[0].nbWeights * sizeof(double));
    uint8_t *sbptr = (uint8_t*) l->neurons;
    sbptr += neuronId * nodeSize;
    return (Neuron*) sbptr;
}

// GET LAYER
Layer *getLayer(Network *nn, LayerType ltype)
{
    Layer *l;
    switch (ltype)
    {
        case INPUT:
        {
            l = nn->layers;
            break;
        }
        case HIDDEN:
        {
            uint8_t *sbptr = (uint8_t*) nn->layers;
            sbptr += nn->inpLayerSize;
            l = (Layer*)sbptr;
            break;
        }
        default:
        {
            uint8_t *sbptr = (uint8_t*) nn->layers;
            sbptr += nn->inpLayerSize + nn->hidLayerSize;
            l = (Layer*)sbptr;
            break;
        }
    }
    return l;
}

// DERIVATE OF ACTIVATION FUNCTION(S)
double dActivation(Network *nn, LayerType ltype, double outVal)
{
    double dVal = 0;
    ActFctType actFct;

    if (ltype==HIDDEN)
        actFct = nn->hidLayerActType;
    else
        actFct = nn->outLayerActType;

    if (actFct==TANH)
        dVal = 1-pow(tanh(outVal),2);
    else
        dVal = outVal * (1-outVal);

    return dVal;
}

// UPDATE WEIGHTS
void updateWeights(Network *nn, LayerType ltype, int id, double error)
{
    Layer *updateLayer = getLayer(nn, ltype);
    Neuron *newNeuron = getNeuron(updateLayer, id);
    Layer *prevLayer;
    int prevLayerNSize = 0;

    if (ltype==HIDDEN) 
    {
        prevLayer = getLayer(nn, INPUT);
        prevLayerNSize = nn->inpNodeSize;
    }
    else
    {
        prevLayer = getLayer(nn, HIDDEN);
        prevLayerNSize = nn->hidNodeSize;
    }

    uint8_t *sbptr = (uint8_t*) prevLayer->neurons;

    for (int i = 0; i < newNeuron->nbWeights; i++)
    {
        Neuron *prevLayerN = (Neuron*)sbptr;
        newNeuron->weights[i] += nn->learningRate * prevLayerN->actv * error;
        sbptr += prevLayerNSize;
    }

    newNeuron->bias += (nn->learningRate * 1 * error);
}

// BACK PROP - HIDDEN LAYER
void backPropHiddenLayer(Network *nn, int target)
{
    Layer *ol = getLayer(nn, OUTPUT);
    Layer *hl = getLayer(nn, HIDDEN);

    for (int h=0;h<hl->ncount;h++)
    {
        Neuron *hn = getNeuron(hl,h);
        double outputErr = 0;

        for (int o=0;o<ol->ncount;o++)
        {
            Neuron *on = getNeuron(ol,o);
            int targetOutput = (o == target) ? 1 : 0;
            double errorD = targetOutput - on->actv;
            double errorS = errorD * dActivation(nn, OUTPUT, on->actv);
            outputErr += errorS * on->weights[h];
        }

        double hiddenErrorS = outputErr * dActivation(nn, HIDDEN, hn->actv);
        updateWeights(nn, HIDDEN, h, hiddenErrorS);
    }
}

// BACK PROP - OUTPUT LAYER
void backPropOutputLayer(Network *nn, int target)
{
    Layer *ol = getLayer(nn, OUTPUT);

    for (int o = 0; o < ol->ncount;o++)
    {
        Neuron *on = getNeuron(ol,o);
        int targetOutput = (o == target) ? 1 : 0;
        double errorD = targetOutput - on->actv;
        double errorS = errorD * dActivation(nn, OUTPUT, on->actv);
        updateWeights(nn, OUTPUT, o, errorS);
    }
}

// BACK PROP
void backProp(Network *nn, int target)
{
    backPropOutputLayer(nn, target);
    backPropHiddenLayer(nn, target);
}

// ACTIVATE NODE
void activateNode(Network *nn, LayerType ltype, int id)
{
    Layer *l = getLayer(nn, ltype);
    Neuron *n = getNeuron(l, id);
    ActFctType actFct;

    if (ltype==HIDDEN) 
        actFct = nn->hidLayerActType;
    else
        actFct = nn->outLayerActType;

    if (actFct==TANH)
        n->actv = tanh(n->actv);
    else
        n->actv = 1 / (1 + (exp((double)-n->actv)));
}

// CALCULATE NODE OUTPUT
void calculateNodeOutput(Network *nn, LayerType ltype, int id)
{
    Layer *calcLayer = getLayer(nn, ltype);
    Neuron *calcNeuron = getNeuron(calcLayer, id);
    Layer *prevLayer;
    int prevLayerNodeSize = 0;

    if (ltype==HIDDEN)
    {
        prevLayer = getLayer(nn, INPUT);
        prevLayerNodeSize = nn->inpNodeSize;
    }
    else
    {
        prevLayer = getLayer(nn, HIDDEN);
        prevLayerNodeSize = nn->hidNodeSize;
    }

    uint8_t *sbptr = (uint8_t*) prevLayer->neurons;
    calcNeuron->actv = calcNeuron->bias;

    for (int i = 0; i < prevLayer->ncount;i++)
    {
        Neuron *prevLayerNeuron = (Neuron*)sbptr;
        calcNeuron->actv += prevLayerNeuron->actv * calcNeuron->weights[i];
        sbptr += prevLayerNodeSize;
    }
}

// CALCULATE LAYER
void calculateLayer(Network *nn, LayerType ltype)
{
    Layer *l;
    l = getLayer(nn, ltype);

    for (int i=0;i<l->ncount;i++)
    {
        calculateNodeOutput(nn, ltype, i);
        activateNode(nn,ltype,i);
    }
}

// FEED FORWARD
void frontProp(Network *nn)
{
    calculateLayer(nn, HIDDEN);
    calculateLayer(nn, OUTPUT);
}

// FEED INPUT
void feedInput(Network *nn, Vector *v)
{
    Layer *il;
    il = nn->layers;
    Neuron *iln;
    iln = il->neurons;

    for (int i=0; i<v->size;i++)
    {
        iln->actv = v->vals[i];
        iln++;
    }
}

// CREATE INPUT LAYER
Layer *createInputLayer(int inpCount)
{
    int inpNodeSize = sizeof(Neuron);
    int inpLayerSize = sizeof(Layer) + (inpCount * inpNodeSize);

    Layer *il = malloc(inpLayerSize);
    il->ncount = inpCount;

    Neuron iln;
    iln.bias = 0;
    iln.actv = 0;
    iln.nbWeights = 0;

    uint8_t *sbptr = (uint8_t*) il->neurons;
    for (int i = 0;i < il->ncount;i++)
    {
        memcpy(sbptr,&iln,inpNodeSize);
        sbptr += inpNodeSize;
    }
    return il;
}

// CREATE LAYER
Layer *createLayer(int nodeCount, int nbWeights)
{
    int nodeSize = sizeof(Neuron) + (nbWeights * sizeof(double));
    Layer *l = (Layer*)malloc(sizeof(Layer) + (nodeCount*nodeSize));
    l->ncount = nodeCount;

    Neuron *dn = (Neuron*)malloc(sizeof(Neuron)+((nbWeights)*sizeof(double)));
    dn->bias = 0;
    dn->actv = 0;
    dn->nbWeights = nbWeights;
    for (int o = 0;o < nbWeights;o++)
        dn->weights[o] = 0;

    uint8_t *sbptr = (uint8_t*) l->neurons;

    for (int i = 0;i < nodeCount;i++)
        memcpy(sbptr+(i*nodeSize),dn,nodeSize);

    free(dn);
    return l;
}

// INIT NETWORK
void initNetwork(Network *nn, int inpCount, int hidCount, int outCount)
{
    Layer *il = createInputLayer(inpCount);
    memcpy(nn->layers,il,nn->inpLayerSize);
    free(il);

    uint8_t *sbptr = (uint8_t*) nn->layers;
    sbptr += nn->inpLayerSize;

    Layer *hl = createLayer(hidCount, inpCount);
    memcpy(sbptr,hl,nn->hidLayerSize);
    free(hl);

    sbptr += nn->hidLayerSize;
    Layer *ol = createLayer(outCount, hidCount);
    memcpy(sbptr,ol,nn->outLayerSize);
    free(ol);
}

// SET DEFAULT PARAMS
void setNetworkDefaults(Network *nn)
{
    nn->hidLayerActType = SIGMOID;
    nn->outLayerActType = SIGMOID;
    nn->learningRate    = 0.2;
}

// INIT WEIGHTS
void initWeights(Network *nn, LayerType ltype)
{
    int nodeSize = 0;
    if (ltype==HIDDEN)
        nodeSize=nn->hidNodeSize;
    else
        nodeSize=nn->outNodeSize;

    Layer *l = getLayer(nn, ltype);
    uint8_t *sbptr = (uint8_t*) l->neurons;

    for (int o = 0; o < l->ncount;o++)
    {
        Neuron *n = (Neuron *)sbptr;

        for (int i = 0; i < n ->nbWeights; i++)
        {
            n->weights[i] = 0.7*(rand()/(double)(RAND_MAX));
            if (i%2) 
                n->weights[i] = -n->weights[i];
        }

        n->bias =  rand()/(double)(RAND_MAX);
        if (o % 2)
            n->bias = -n->bias;
        sbptr += nodeSize;
    }
}

// CREATE NETWORK
Network *createNetwork(int inpCount, int hidCount, int outCount)
{
    int inpNodeSize = sizeof(Neuron);
    int inpLayerSize = sizeof(Layer) + (inpCount * inpNodeSize);

    int hidWeightsCount = inpCount;
    int hidNodeSize = sizeof(Neuron) + (hidWeightsCount * sizeof(double));
    int hidLayerSize = sizeof(Layer) + (hidCount * hidNodeSize);

    int outWeightsCount = hidCount;
    int outNodeSize = sizeof(Neuron) + (outWeightsCount * sizeof(double));
    int outLayerSize = sizeof(Layer) + (outCount * outNodeSize);

    int sum = inpLayerSize + hidLayerSize + outLayerSize;
    Network *nn = (Network*)malloc(sizeof(Network) + sum);

    nn->inpNodeSize = inpNodeSize;
    nn->inpLayerSize = inpLayerSize;
    nn->hidNodeSize = hidNodeSize;
    nn->hidLayerSize = hidLayerSize;
    nn->outNodeSize = outNodeSize;
    nn->outLayerSize = outLayerSize;

    initNetwork(nn, inpCount, hidCount, outCount);

    setNetworkDefaults(nn);

    initWeights(nn, HIDDEN);
    initWeights(nn, OUTPUT);

    return nn;
}

// GET RESULT
int getResult(Network *nn)
{
    Layer *l = getLayer(nn, OUTPUT);
    double maxOut = 0;
    int maxInd = 0;
    for (int i=0; i<l->ncount; i++)
    {
        Neuron *n = getNeuron(l,i);
        if (n->actv > maxOut)
        {
            maxOut = n->actv;
            maxInd = i;
        }
    }
    return maxInd;
}
