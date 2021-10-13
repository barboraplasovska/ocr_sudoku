#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "layer.h"
#include "neuron.h"
#include "network.h"

// --------------------------------------
// ----------------MAIN------------------
// --------------------------------------
int main()
{
    if (initNetwork() == 1)
    {
        printf("Error: error in initialization of the network.\n");
        return 1;
    }    
    else
        printf("Network initialized successfuly. \n");

    printData();
    train();
    test();

    return 0;
}

