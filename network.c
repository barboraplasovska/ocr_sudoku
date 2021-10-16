#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif // defines getDelim

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "layer.h"
#include "neuron.h"

// --------------------------------------
// --------------VARIABLES---------------

Layer *layer = (void *)0; 
const int nbLayers = 3;
int nbNeurons[3] = {2,2,1};
float lr = 0.8;
float *cost;
float fullCost;

float input[8][4] = {{1,1}, {0,0}, {1,0}, {0,1}};
float expectedOutputs[] = {0, 0, 1, 1};
int nbTrainingInputs = 4;

int n = 1;

// --------------------------------------
// --------------FUNCTIONS---------------

// INITIALIZE NETWORK
int initNetwork(void)
{
    // init Layers
    layer = (Layer*) malloc(nbLayers * sizeof(layer));

    cost = (float *) malloc(nbNeurons[nbLayers-1] * sizeof(float));
    memset(cost,0,nbNeurons[nbLayers-1]*sizeof(float));

    for(int i=0;i<nbLayers;i++)
    {
        layer[i] = initLayer(nbNeurons[i]);      
        layer[i].nbNeurons = nbNeurons[i];

        for(int j=0;j<nbNeurons[i];j++)
        {
            if(i < (nbLayers-1)) 
            {
                layer[i].neuron[j] = initNeuron(nbNeurons[i+1]);
            }
        }
    }

    if(layer == (void *)0) 
    {
        return 1; // ERROR -> No layers in the network
    }

    return 0;
}

void initWeights(void)
{
    // init Weights and Biases

    for(int i=0;i<nbLayers-1;i++)
    {

        for(int j=0;j<nbNeurons[i];j++)
        {

            for(int k=0;k<nbNeurons[i+1];k++)
            {
                double rnd = ((double)rand())/((double)RAND_MAX);
                layer[i].neuron[j].weights[k] = rnd;
                layer[i].neuron[j].dWeights[k] = 0.0;
            }

            if(i>0) 
            {
                double rnd = ((double)rand())/((double)RAND_MAX);
                layer[i].neuron[j].bias = rnd;
            }
        }
    }

    for (int j=0; j<nbNeurons[nbLayers-1]; j++)
    {
        double rnd = ((double)rand())/((double)RAND_MAX);
        layer[nbLayers-1].neuron[j].bias = rnd;
    }
}

// FEED INPUT
void feedInput(int p)
{
    for(int j=0;j<nbNeurons[0];j++)
    {
        layer[0].neuron[j].actv = input[p][j];
    }

}

// FORWARD PROPAGATION
void forwardProp(void)
{
    for(int i = 1 ; i < nbLayers; i++)
    {
        for(int j = 0 ; j < nbNeurons[i]; j++)
        {
            layer[i].neuron[j].z = layer[i].neuron[j].bias;

            for(int k = 0 ; k < nbNeurons[i-1]; k++)
            {
                float w = layer[i-1].neuron[k].weights[j];
                float a = layer[i-1].neuron[k].actv;
                layer[i].neuron[j].z += w * a;
            }

            if(i < nbLayers-1) // Relu Activation Function for Hidden Layers
            {
                if((layer[i].neuron[j].z) < 0)
                    layer[i].neuron[j].actv = 0;
                else
                    layer[i].neuron[j].actv = layer[i].neuron[j].z;
            }
            else
            {
                layer[i].neuron[j].actv = 1/(1+exp(-layer[i].neuron[j].z));
            }
        }
    }
}

// CALCULATE COSTS
void calculateCosts(int p)
{
    float tmpcost = 0;
    float c = 0;

    for(int j=0; j < nbNeurons[nbLayers-1]; j++)
    {
        tmpcost = expectedOutputs[p] - layer[nbLayers-1].neuron[j].actv;
        cost[j] = (tmpcost * tmpcost)/2;
        c = c + cost[j];
    }

    fullCost = (fullCost + c)/n;
    n++;
}

// BACK PROPAGATION
void backProp(int p)
{
    // Output Layer
    for(int j = 0; j < nbNeurons[nbLayers-1]; j++)
    {
        float diff = layer[nbLayers-1].neuron[j].actv - expectedOutputs[p];
        float actv = layer[nbLayers-1].neuron[j].actv;
        layer[nbLayers-1].neuron[j].dZ = diff * actv * (1 - actv);

        for(int k=0; k < nbNeurons[nbLayers-2]; k++)
        {
            float dZ = layer[nbLayers-1].neuron[j].dZ;
            float a = layer[nbLayers-2].neuron[k].actv;
            float w = layer[nbLayers-2].neuron[k].weights[j];

            layer[nbLayers-2].neuron[k].dWeights[j] = dZ * a;
            layer[nbLayers-2].neuron[k].dActv =  dZ * w;
        }

        layer[nbLayers-1].neuron[j].dBias = layer[nbLayers-1].neuron[j].dZ;
    }

    // Hidden Layers
    for(int i = nbLayers-2; i > 0 ; i--)
    {
        for(int j=0;j<nbNeurons[i];j++)
        {
            if(layer[i].neuron[j].z >= 0)
            {
                layer[i].neuron[j].dZ = layer[i].neuron[j].dActv;
            }
            else
            {
                layer[i].neuron[j].dZ = 0;
            }

            for(int k=0; k < nbNeurons[i-1];k++)
            {
                float dZ = layer[i].neuron[j].dZ;
                float a = layer[i-1].neuron[k].actv;
                float w = layer[i-1].neuron[k].weights[j];

                layer[i-1].neuron[k].dWeights[j] = dZ * a;

                if(i>1)
                {
                    layer[i-1].neuron[k].dActv = w * dZ;
                }
            }

            layer[i].neuron[j].dBias = layer[i].neuron[j].dZ;
        }
    }
}

// UPDATE WEIGHTS
void updateWeights(void)
{
    for(int i = 0 ;i < nbLayers-1;i++)
    {
        for(int j=0; j < nbNeurons[i];j++)
        {
            float d;
            for(int k=0; k < nbNeurons[i+1];k++)
            {
                // Update Weights
                d = lr * layer[i].neuron[j].dWeights[k];
                layer[i].neuron[j].weights[k] -= d;
            }

            // Update Bias
            d = lr * layer[i].neuron[j].dBias;
            layer[i].neuron[j].bias -= d;
        }
    }
}

// TRAIN NEURAL NETWORK
void train(void)
{

    for(int nb=0;nb<20000;nb++)
    {
        for(int i=0;i<nbTrainingInputs;i++)
        {

            feedInput(i);
            forwardProp();
            calculateCosts(i);
            backProp(i);
            updateWeights();
        }
    }

}

// PRINT DATA
void printData(void)
{
    printf("\n");
    printf("Initial data used: \n");
    printf("Number of layers: %d\n", nbLayers);

    for (int i = 0; i < nbLayers; i++)
    {
        printf("Layer %d: %d neurons \n", i, nbNeurons[i]);
    }

    printf("Learning rate: %f \n", lr);

    printf("Training inputs: {{0,0}, {0,1}, {1,0}, {1,1}} \n");
    printf("Expected training outputs: {0, 1, 1, 0} \n");

    printf("\n");

}

// TEST NEURAL NETWORK
void test(void)
{
    printf("TEST\n");
    printf("\n");
    for (int n = 0; n < 4; n++)
    {
        printf("Enter input to test:\n");

        for(int i=0; i < nbNeurons[0] ;i++)
        {
            scanf("%f",&layer[0].neuron[i].actv);
        }
        forwardProp();

        printf("Output: %d\n", (int)round(layer[2].neuron[0].actv));
        printf("\n");
    }
}

// SAVE WEIGHTS
void saveWeights()
{
    FILE *fptr;
    fptr = fopen("weights.txt","w");

    if(fptr == NULL)
    {
        printf("Error!");   
        exit(1);             
    }


    for(int i = 0 ;i < nbLayers-1;i++)
    {
        for(int j=0; j < nbNeurons[i];j++)
        {
            for(int k=0; k < nbNeurons[i+1];k++)
            {
                // weights
                float w = layer[i].neuron[j].weights[k];
                fprintf(fptr,"%f,",w);
            }
            fprintf(fptr,"|");

            // Bias
            float b = layer[i].neuron[j].bias;
            fprintf(fptr,"%f+\n", b);
        }
        if (i == nbLayers-2)
            fprintf(fptr,"+++");
        else
            fprintf(fptr,"+\n");
    }
    fclose(fptr);
}

void reinitArr(char *temp)
{
    for (size_t i = 0; i < 8; i++)
    {
        if (i == 1)
            temp[i] = '.';
        else
            temp[i] = '0';
    }
}

// LOAD WEIGHTS

void loadWeights(char *filePath)
{
    FILE *fptr = fopen(filePath,"r");

    if (fptr == NULL)
    {
       printf("Error! opening file");
       exit(1);
    }

    char *l;
    int read;
    size_t len;


    int lay = 0;
    size_t weightInd = 0;
    size_t neuronInd = 0;
    size_t tempInd = 0;
    size_t i = 0;
    int negative = 0;
    char *temp;
    temp = (char *) malloc(8 * sizeof(char));

    while ((read = getdelim(&l, &len, '\n', fptr)) != EOF)
    {
        if (l[0] == '+' && l[1] == '+')
        {
            break;
        }
        else
        {
            if(l[0] == '+')
            {
                if (l[1] == '\n')
                {
                    // next layer
                    lay++;
                    weightInd = 0;
                    neuronInd = 0;
                    tempInd = 0;
                    i = 0;
                    negative = 0;
                    reinitArr(temp);
                }
            }
            else
            {
                i = 0;
                tempInd = 0;
                // weights
                while (i < len && l[i] != '|')
                {
                    if (l[i] == ',')
                    {
                        //save temp
                        double f = atof(temp);
                        if (negative == 1)
                        {
                            f *= -1.0;
                        }
                        printf("layer[%d].neuron[%zu].weights[%zu] = %s\n", lay, neuronInd, weightInd, temp);
                        layer[lay].neuron[neuronInd].weights[weightInd] = (float)f;
                        weightInd++;
                    
                        // empty temp
                        reinitArr(temp);
                        tempInd = 0;
                        negative = 0;
                    }
                    else
                    {
                        if (l[i] == '-')
                            negative = 1;
                        else 
                        {
                            temp[tempInd] = l[i];
                            tempInd++;
                        }
                    }   
                    i++;
                }
                i++;
                tempInd = 0;
                reinitArr(temp);
                
                // bias
                while (i < len && l[i] != '\n')
                {
                    if (l[i] == '+')
                    {
                        // save temp
                        double f = atof(temp);
                        if (negative == 1)
                        {
                            f *= -1.0;
                        }
                        printf("layer[%d].neuron[%zu].bias = %s\n", lay, neuronInd, temp);
                        layer[lay].neuron[neuronInd].bias = (float)f;

                        // empty temp
                        reinitArr(temp);
                        tempInd = 0;
                        negative = 0;
                    }
                    else
                    {
                        if (l[i] == '-')
                            negative = 1;
                        else 
                        {
                            temp[tempInd] = l[i];
                            tempInd++;
                        }
                    }
                    i++;
                }
                // we go to the next neuron
                weightInd = 0;
                negative = 0;
                neuronInd++;
            }
        }

    }

    layer[lay+1].nbNeurons = 1;

    fclose(fptr);
    free(temp);
}

void dinit()
{
    // free all the structures

    free(layer);
}

