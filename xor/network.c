#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "layer.h"
#include "neuron.h"

// --------------------------------------
// --------------VARIABLES---------------

Layer *layer = (void *)0; 
int nbLayers = 3;
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
    int i=0,j=0;
    int k=0;
    layer = (Layer*) malloc(nbLayers * sizeof(layer));

    cost = (float *) malloc(nbNeurons[nbLayers-1] * sizeof(float));
    memset(cost,0,nbNeurons[nbLayers-1]*sizeof(float));

    for(i=0;i<nbLayers;i++)
    {
        layer[i] = initLayer(nbNeurons[i]);      
        layer[i].nbNeurons = nbNeurons[i];

        for(j=0;j<nbNeurons[i];j++)
        {
            if(i < (nbLayers-1)) 
            {
                layer[i].neuron[j] = initNeuron(nbNeurons[i+1]);
            }
        }
    }

    // init Biases
    if(layer == (void *)0) 
    {
        return 1; // ERROR -> No layers in the network
    }

    for(i=0;i<nbLayers-1;i++)
    {

        for(j=0;j<nbNeurons[i];j++)
        {

            for(k=0;k<nbNeurons[i+1];k++)
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

    for (j=0; j<nbNeurons[nbLayers-1]; j++)
    {
        double rnd = ((double)rand())/((double)RAND_MAX);
        layer[nbLayers-1].neuron[j].bias = rnd;
    }

    return 0;
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
            else // Sigmoid Activation function for Output Layer
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
            float actv = layer[nbLayers-2].neuron[k].actv;

            layer[nbLayers-2].neuron[k].dWeights[j] = dZ * actv;

            float w = layer[nbLayers-2].neuron[k].weights[j];
            layer[nbLayers-2].neuron[k].dActv =  w * dZ;
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
                Neuron n2 = layer[i-1].neuron[k];

                layer[i-1].neuron[k].dWeights[j] = dZ * n2.actv;

                if(i>1)
                {
                    layer[i-1].neuron[k].dActv = n2.weights[j] * dZ;
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
    while(1)
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
