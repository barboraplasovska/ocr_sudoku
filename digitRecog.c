#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif // defines getDelim

#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "network.h"
#include "image_processing.h" //not sure it should be here

double getBWPixel(SDL_Surface *surface, unsigned x, unsigned y)
{
    Uint32 pixel = get_pixel(surface, x, y);
    Uint8 r, g, b;
    SDL_GetRGB(pixel, surface->format, &r, &g, &b);
    
    if(r == 255)
    {
        r = 0;
    }
    else
    {
        r = 1;
    }
    return r;
}

Vector *getVectorFromImage(SDL_Surface *img)
{
    Vector *v = (Vector*)malloc(sizeof(Vector) + (28*28 * sizeof(double)));

    //SDL_Surface* imgg = SDL_CreateRGBSurface(0,28,28,32,0,0,0,0);
    
    v->size = 28*28;
    size_t i = 0;
    
    for (size_t y = 0; y < 28; y++)
    {
        for (size_t x = 0; x < 28; x++)
        {
        /*
            if (getBWPixel(img, x, y) == 0)
            {
                int r1 = 255;
                Uint32 pixel = SDL_MapRGB(img->format, r1,r1,r1);
                put_pixel(imgg,x,y,pixel);
            }
            else
            {
                int r1 = 0;
                Uint32 pixel = SDL_MapRGB(img->format, r1,r1,r1);
                put_pixel(imgg,x,y,pixel);
            }*/
        
            v->vals[i] = getBWPixel(img, x, y);
            //printf("Value got is: %f\n", getBWPixel(img, x, y));
            if (getBWPixel(img,x,y) != 0 && getBWPixel(img, x, y) != 1)
            {
                printf("there is a problem\n");
            }
            i++;
        }
    }
    
    //SDL_SaveBMP(img, "TEST.bmp");
    
    printf("finished\n");
    
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

    size_t nbNeurons[3] = {784,20,10};
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

void displayNetworkWeightsForDebugging(Network *nn){
    
    // only print the first x and last x nodes/connections (to improve legible rendering in the console screen)
    int topLast = 6;
    
    for (int l=1; l<2;l++){
        
        Layer *layer = getLayer(nn, OUTPUT);
        
        printf("Layer %d   Weights\n\n",l);
        
        // print connections per node and WEIGHT of connection
        
        if (layer->ncount>0){
            
            printf("Layer %d   NodeId  |  ConnectionId:WeightAddress \n\n",l);
            
            int kSize = 5*5;
            
            // table header
            printf("Node | ");
            for (int x=0; x<kSize; x++) {if (x<topLast || x>=kSize-topLast) printf(" conn:address  ");}
            printf("\n-------");
            for (int x=0; x<kSize; x++) {if (x<topLast || x>=kSize-topLast) printf("---------------");}
            printf("\n");
            
            for (int n=0; n<10; n++){
                
                printf("%4d | ",n);
                
                Neuron *node = getNeuron(layer, n);
                
                int connCount = node->nbWeights;
                
                for (int c=0; c<connCount; c++){
                    
                    // Dereference the weightPointer to validate its pointing to a valid weight
                    double w = node->weights[c];
                    
                    if (c<topLast || c>=connCount-topLast) printf("%5d:%9f",c,w);
                }
                printf("\n");
                
                
            }
            printf("\n\n");
            
            
        }
        
    }
    
}

// LOAD WEIGHTS
void loadWeights(char *filePath, Network *nn)
{
    FILE *fptr = fopen(filePath,"r");
    printf("opened file\n");

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
    size_t nbNeurons[3] = {784,20,10};
    LayerType lTypes[3] = {INPUT, HIDDEN, OUTPUT};

    Layer *layer = getLayer(nn, lTypes[layInd]);
    Neuron *neuron = getNeuron(layer, neuInd);

    while ((read = getline(&line, &len, fptr)) != -1)
    {
        if (line[0] == '*')
        {
            
            if (layInd < 2)
            {
                layInd++;
            }
            	
            neuInd = 0;
            weInd = 0;
            bias = 1;
            printf("before get layer here\n");
            layer = getLayer(nn, lTypes[layInd]);
            printf("after get layer here\n");
            neuron = getNeuron(layer, neuInd);
            printf("after get neuron here\n");
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
    //printf("finding digit 1\n");
    Vector *inpVector = getVectorFromImage(image);
    //printf("finding digit 2\n");
    feedInput(nn, inpVector);
    //printf("finding digit 3\n");
    frontProp(nn);
    //printf("finding digit 4\n");

    int classification = getResult(nn);
    printf("finding digit 5\n");
    return classification;
}

// SETUP NETWORK BEFORE FINDING NUMBERS
Network* setupNetwork(void)
{
    Network *nn = createNetwork(784, 20, 10);
    //displayNetworkWeightsForDebugging(nn);
    printf("before load\n");
    loadWeights("weights.txt", nn);
    printf("after load\n");
    return nn;
}

// SHUT DOWN NETWORK
void shutDownNetwork(Network *nn)
{
    saveWeights(nn);
    free(nn);
}
