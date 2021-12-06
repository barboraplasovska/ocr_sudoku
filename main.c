#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif // defines getDelim

#include <stdlib.h>
//#include </opt/homebrew/include/SDL/SDL.h>
//#include </opt/homebrew/include/SDL/SDL_image.h>
//#include <SDL/SDL.h>
//#include <SDL/SDL_image.h>
#include "network.h"
//#include "image_processing.h" //not sure it should be here
#include "screen.h" 
#include "mnist-utils.h"
#include "mnist-stats.h"  

/**
 * @brief Returns a Vector holding the image pixels of a given MNIST image
 * @param img A pointer to a MNIST image
 */

Vector *getVectorFromImage(MNIST_Image *img){
    
    Vector *v = (Vector*)malloc(sizeof(Vector) + (MNIST_IMG_WIDTH*MNIST_IMG_HEIGHT * sizeof(double)));
    
    v->size = MNIST_IMG_WIDTH*MNIST_IMG_HEIGHT;
    
    for (int i=0;i<v->size;i++)
        v->vals[i] = img->pixel[i] ? 1 : 0;
    
    return v;
}




/**
 * @brief Training the network by processing the MNIST training set and updating the weights
 * @param nn A pointer to the NN
 */

void trainNetwork(Network *nn){
    
    // open MNIST files
    FILE *imageFile, *labelFile;
    imageFile = openMNISTImageFile(MNIST_TRAINING_SET_IMAGE_FILE_NAME);
    labelFile = openMNISTLabelFile(MNIST_TRAINING_SET_LABEL_FILE_NAME);
    
    int errCount = 0;

    // Loop through all images in the file
    for (int imgCount=0; imgCount<MNIST_MAX_TRAINING_IMAGES; imgCount++){
        
        // Reading next image and its corresponding label
        MNIST_Image img = getImage(imageFile);
        MNIST_Label lbl = getLabel(labelFile);
        
        // Convert the MNIST image to a standardized vector format and feed into the network
        Vector *inpVector = getVectorFromImage(&img);
        feedInput(nn, inpVector);
        
        // Feed forward all layers (from input to hidden to output) calculating all nodes' output
        frontProp(nn);
        
        // Back propagate the error and adjust weights in all layers accordingly
        backProp(nn, lbl);
        
        // Classify image by choosing output cell with highest output
        int classification = getResult(nn);
        if (classification!=lbl) errCount++;
        
        // Display progress during training
        displayTrainingProgress(0,imgCount, errCount, 3,5);
//        displayImage(&img, lbl, classification, 7,6);

    }
    
    // Close files
    fclose(imageFile);
    fclose(labelFile);
    
}




/**
 * @brief Testing the trained network by processing the MNIST testing set WITHOUT updating weights
 * @param nn A pointer to the NN
 */

void testNetwork(Network *nn){
    
    // open MNIST files
    FILE *imageFile, *labelFile;
    imageFile = openMNISTImageFile(MNIST_TESTING_SET_IMAGE_FILE_NAME);
    labelFile = openMNISTLabelFile(MNIST_TESTING_SET_LABEL_FILE_NAME);
    
    int errCount = 0;
    
    // Loop through all images in the file
    for (int imgCount=0; imgCount<MNIST_MAX_TESTING_IMAGES; imgCount++){
        
        // Reading next image and its corresponding label
        MNIST_Image img = getImage(imageFile);
        MNIST_Label lbl = getLabel(labelFile);
        
        // Convert the MNIST image to a standardized vector format and feed into the network
        Vector *inpVector = getVectorFromImage(&img);
        feedInput(nn, inpVector);
        
        // Feed forward all layers (from input to hidden to output) calculating all nodes' output
        frontProp(nn);
        
        // Classify image by choosing output cell with highest output
        int classification = getResult(nn);
        if (classification!=lbl) errCount++;
        
        // Display progress during testing
        displayTestingProgress(imgCount, errCount, 5,5);
//        displayImage(&img, lbl, classification, 7,6);
        
    }
    
    // Close files
    fclose(imageFile);
    fclose(labelFile);
    
}

/*
double getBWPixel(SDL_Surface *surface, unsigned x, unsigned y)
{
    Uint32 pixel = get_pixel(surface, x, y);
    Uint8 r, g, b;
    SDL_GetRGB(pixel, surface->format, &r, &g, &b);
            
    if(r == 255)
        r = 0;
    else
        r = 1;
    return r;
}



Vector *getVectorFromImage(SDL_Surface *img)
{
    Vector *v = (Vector*)malloc(sizeof(Vector) + (28*28 * sizeof(double)));

    v->size = 28*28;
    size_t i = 0;
    for (size_t y = 0; y < 28; y++)
    {
        for (size_t x = 0; x < 28; x++)
        {
            v->vals[i] = getBWPixel(img, x, y);
            i++;
        }
    }
    return v;
}*/

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
    size_t nbNeurons[3] = {784,20,10};
    LayerType lTypes[3] = {INPUT, HIDDEN, OUTPUT};

    Layer *layer = getLayer(nn, lTypes[layInd]);
    Neuron *neuron = getNeuron(layer, neuInd);
    int i = 0;

    while ((read = getline(&line, &len, fptr)) != -1)
    {
        //printf("%d: %s", i, line);
        i++;
        if (line[0] == '*')
        {
			if (layInd < 2)
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

/*
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


// TRAIN WITH MNIST
void train(Network *nn)
{
    char boxPath[] = "train/Boxxy.bmp";
    size_t index = 0;
    int errCount = 0;
    int label[] = {5,6,7,8,4,3,9,6,8,1,8,4,7,9,6,2,1,8,5,3,9,2,6,8,7,3,1,6,5,9 //30
        ,8,1,2,6,1,5,4,8,9,1,7,6,8,5,3,2,2,7,6,9,5,4,1,2,3,9,3,6,8,1,6,2,5,3,2, //36
        5,6,8,9,5,8,3,4,1,6,9,7,1,4,5,4,6,2,1,7,6,3,3,7,1,3,6,2,4,6,1,4,6,5,3,7,
        5,4,3,6,9,3,8,2,4,4,5,1,3,6,5,2,1,4,2,9,9,5,7,8,2,4,1,6,7,5,7,2,9,1,4,2,8,
        4,2,3,1,6,3,9,9,2,3,8,6,5,9,1,4,2,9,1,5,7,3};

    for (size_t epoch = 0; epoch < 200; epoch++)
    {
        index = 0;
        errCount = 0;
        for (size_t x = 0; x < 16; x++)
        {
            for (size_t y = 0; y < 10; y++)
            {

                switch (x)
                {
                case 10:
                    boxPath[9] = 'A';
                    break;
                case 11:
                    boxPath[9] = 'B';
                    break;
                case 12:
                    boxPath[9] = 'C';
                    break;
                case 13:
                    boxPath[9] = 'D';
                    break;
                case 14:
                    boxPath[9] = 'E';
                    break;
                case 15:
                    boxPath[9] = 'F';
                    break;
                default:
                    boxPath[9] = x + '0';
                    break;
                }
                boxPath[10] = y + '0';
                SDL_Surface *img = load_image(boxPath);

                Vector *inpVector = getVectorFromImage(img);

                feedInput(nn, inpVector);
        
                frontProp(nn);

                backProp(nn, label[index]);
        
                int classification = getResult(nn);
                if (classification != label[index]) 
                    errCount++;
        
                displayTrainingProgress(epoch, index, errCount, 3, 5);

                index++;
            }
            
        }
        
    }
}

// TEST NETWORK WITH MNIST
void test(Network *nn)
{
    char boxPath[] = "train/Boxxy.bmp";
    size_t index = 0;
    int errCount = 0;
    int label[] = {5,6,7,8,4,3,9,6,8,1,8,4,7,9,6,2,1,8,5,3,9,2,6,8,7,3,1,6,5,9
        ,8,1,2,6,1,5,4,8,9,1,7,6,8,5,3,2,2,7,6,9,5,4,1,2,3,9,3,6,8,1,6,2,5,3,2,
        5,6,8,9,5,8,3,4,1,6,9,7,1,4,5,4,6,2,1,7,6,3,3,7,1,3,6,2,4,6,1,4,6,5,3,7,
        5,4,3,6,9,3,8,2,4,4,5,1,3,6,5,2,1,4,2,9,9,5,7,8,2,4,1,6,7,5,7,2,9,1,4,2,8,
        4,2,3,1,6,3,9,9,2,3,8,6,5,9,1,4,2,9,1,5,7,3};

    for (size_t epoch = 0; epoch < 1; epoch++)
    {
        index = 0;
        errCount = 0;
        for (size_t x = 0; x < 16; x++)
        {
            for (size_t y = 0; y < 10; y++)
            {

                switch (x)
                {
                case 10:
                    boxPath[9] = 'A';
                    break;
                case 11:
                    boxPath[9] = 'B';
                    break;
                case 12:
                    boxPath[9] = 'C';
                    break;
                case 13:
                    boxPath[9] = 'D';
                    break;
                case 14:
                    boxPath[9] = 'E';
                    break;
                case 15:
                    boxPath[9] = 'F';
                    break;
                default:
                    boxPath[9] = x + '0';
                    break;
                }
                boxPath[10] = y + '0';
                SDL_Surface *img = load_image(boxPath);

                Vector *inpVector = getVectorFromImage(img);

                feedInput(nn, inpVector);
        
                frontProp(nn);
        
                int classification = getResult(nn);
                if (classification != label[index]) 
                    errCount++;
        
                displayTrainingProgress(epoch, index, errCount, 3, 5);
                index++;
            }
        }
    }
}*/

// MAIN
int main()
{
    
    // remember the time in order to calculate processing time at the end
    //time_t startTime = time(NULL);
    
    // clear screen of terminal window
    clearScreen();
    
    // Create neural network using a manually allocated memory space
    Network *nn = createNetwork(28*28, 20, 10);

    // load weights
    //loadWeights("weights.txt", nn);

    // for training
    //train(nn);
    trainNetwork(nn); // with mnist
    //test(nn);
    testNetwork(nn); // with mnist

    saveWeights(nn);
    
    // Free the manually allocated memory for this network
    free(nn);
    
    locateCursor(36, 5);
    
    // Calculate and print the program's total execution time
    //time_t endTime = time(NULL);
    //double executionTime = difftime(endTime, startTime);
    //printf("\n    DONE! Total execution time: %.1f sec\n\n",executionTime);

    return 0;
}