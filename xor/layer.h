#ifndef LAYER_H
#define LAYER_H

#include "neuron.h"

typedef struct layer_t
{
    int nbNeurons;
    struct neuron_t *neuron; 
} Layer;

Layer initLayer(int nbNeurons);

#endif
