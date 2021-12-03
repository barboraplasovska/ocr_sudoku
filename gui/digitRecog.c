#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif // defines getDelim

#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "network.h"
#include "image_processing.h" //not sure it should be here

static inline Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y)
{
    int bpp = surf->format->BytesPerPixel;
    return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}

double getBWPixel(SDL_Surface *surface, unsigned x, unsigned y)
{
    Uint32 pixel = get_pixel(surface, x, y);
    Uint8 r, g, b;
    SDL_GetRGB(pixel, surface->format, &r, &g, &b);
            
    if(r == 255)
        r = 1;
    return r;
}



Vector *getVectorFromImage(SDL_Surface *img)
{
    Vector *v = (Vector*)malloc(sizeof(Vector) + (28*28 * sizeof(double)));

    v->size = 28*28;
    size_t i = 0;
    for (size_t x = 0; x < 28; x++)
    {
        for (size_t y = 0; y < 28; y++)
        {
            v->vals[i] = getBWPixel(img, x, y);
            i++;
        }
    }
    return v;
}

// SAVE WEIGHTS
void saveWeights(Network *nn)
{
    FILE *fptr;
    fptr = fopen("weights.txt","w");

    if(fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }

    size_t nbNeurons[3] = {783,20,10};
    LayerType lTypes[3] = {INPUT, HIDDEN, OUTPUT};

    for (size_t l = 0; l < 3; l++)
    {
        Layer *layer = getLayer(nn, lTypes[l]);
        for (size_t i = 0; i < nbNeurons[l]; i++)
        {
            Neuron *neuron = getNeuron(layer, i);
            fprintf(fptr,"%f\n", neuron->bias);
            if (l != 0)
            {
                for (size_t k = 0; k < nbNeurons[l-1]; k++)
                {
                    fprintf(fptr,"%f\n", neuron->weights[k]);
                }
            }
        }

        fprintf(fptr,"*\n");
    }
    fclose(fptr);
}


double getNb(char *line)
{
    int negative = 0;
    char *eptr;
    double res = strtod(line, &eptr);
    if (negative == 1)
        return (-1.0) * res;
    return res;
}

// LOAD WEIGHTS
void loadWeights(char *filePath, Network *nn)
{
    FILE *fptr = fopen(filePath,"r");

    if (fptr == NULL)
    {
       printf("Error! opening file");
       exit(1);
    }

    char *line;
    int read;
    size_t len;
    size_t layInd = 0;
    size_t neuInd = 0;
    size_t weInd = 0;
    int bias = 1;
    size_t nbNeurons[3] = {783,20,10};
    LayerType lTypes[3] = {INPUT, HIDDEN, OUTPUT};

    Layer *layer = getLayer(nn, lTypes[layInd]);
    Neuron *neuron = getNeuron(layer, neuInd);

    while ((read = getdelim(&line, &len, '\n', fptr)) != EOF)
    {
        if (line[0] == '*')
        {
            layInd++;
            neuInd = 0;
            weInd = 0;
            bias = 1;
            layer = getLayer(nn, lTypes[layInd]);
            neuron = getNeuron(layer, neuInd);
        }
        else
        {
            if (layInd == 0)
            {
                neuron->bias = getNb(line);
                neuInd++;
                neuron = getNeuron(layer, neuInd);
            }
            else
            {
                if (weInd > nbNeurons[layInd-1])
                {
                    neuInd++;
                    neuron = getNeuron(layer, neuInd);
                    weInd = 0;
                }

                if (bias == 1)
                {
                    neuron->bias = getNb(line);
                    bias = 0;
                }
                else
                {
                    neuron->weights[weInd] = getNb(line);
                    weInd++;
                }
            }
        }
    }
}

// FIND DIGIT
int findDigit(Network *nn, SDL_Surface *image)
{
    Vector *inpVector = getVectorFromImage(image);
    feedInput(nn, inpVector);
    frontProp(nn);

    int classification = getResult(nn);
    return classification;
}

// SETUP NETWORK BEFORE FINDING NUMBERS
Network* setupNetwork(void)
{
    Network *nn = createNetwork(784, 20, 10);
    loadWeights("weights.txt", nn);
    return nn;
}

// SHUT DOWN NETWORK
void shutDownNetwork(Network *nn)
{
    saveWeights(nn);
    free(nn);
}
