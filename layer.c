#include "layer.h"
#include <stdlib.h>


Layer initLayer(int nbNeurons)
{
    Layer layer;
    layer.nbNeurons = -1;
    layer.neuron = (struct neuron_t *) malloc(nbNeurons * sizeof(struct neuron_t));
    return layer;
}
