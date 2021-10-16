#include "neuron.h"
#include <stdio.h>
#include <stdlib.h>


Neuron initNeuron(int nbOutputWeights)
{
    Neuron neuron;

    neuron.actv = 0.0;
    neuron.weights = (float*) malloc(nbOutputWeights * sizeof(float));
    neuron.bias= 0.0;
    neuron.z = 0.0;
    
    neuron.dActv = 0.0;
    neuron.dWeights = (float*) malloc(nbOutputWeights * sizeof(float));
    neuron.dBias = 0.0;
    neuron.dZ = 0.0;

    return neuron;
}

