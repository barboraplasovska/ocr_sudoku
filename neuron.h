#ifndef NEURON_H
#define NEURON_H

typedef struct neuron_t
{
    float actv;
    float *weights;
    float bias;
    float z;

    float dActv;
    float *dWeights;
    float dBias;
    float dZ;

} Neuron;

Neuron initNeuron(int nbOutputWeights);

#endif
