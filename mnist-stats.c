#include <string.h>
#include "mnist-utils.h"
#include "mnist-stats.h"
#include "screen.h"

void displayImageFrame(int row, int col)
{    
    if (col!=0 && row!=0) locateCursor(row, col);

    printf("------------------------------\n");
    
    for (int i=0; i<MNIST_IMG_HEIGHT; i++){
        for (int o=0; o<col-1; o++) printf(" ");
        printf("|                            |\n");
    }

    for (int o=0; o<col-1; o++) printf(" ");
    printf("------------------------------\n");
    
}

void displayImage(MNIST_Image *img, int lbl, int cls, int row, int col)
{

    char imgStr[(MNIST_IMG_HEIGHT * MNIST_IMG_WIDTH)+((col+1)*MNIST_IMG_HEIGHT)+1];
    strcpy(imgStr, "");
    
    for (int y=0; y<MNIST_IMG_HEIGHT; y++)
    {
        
        for (int o=0; o<col-2; o++) 
            strcat(imgStr," ");
        
        for (int x=0; x<MNIST_IMG_WIDTH; x++)
        {
            strcat(imgStr, img->pixel[y*MNIST_IMG_HEIGHT+x] ? "X" : "." );
        }
        strcat(imgStr,"\n");
    }
    
    if (col!=0 && row!=0) 
        locateCursor(row, 0);
    printf("%s",imgStr);
    printf("     Label:%d   Classification:%d\n\n",lbl,cls);
}

void displayTrainingProgress(size_t epoch, int imgCount, int errCount, int y, int x)
{    
    double p = (double)(imgCount+1)/(double)(MNIST_MAX_TRAINING_IMAGES)*100;
    
    if (x!=0 && y!=0) locateCursor(y, x);
    
    printf("Epoch %lu: TRAINING: Reading image No. %5d", epoch, (imgCount+1));
    printf(" of %5d images [%3d%%]  ",MNIST_MAX_TRAINING_IMAGES,(int)p);

    
    double a = 1 - ((double)errCount/(double)(imgCount+1));
    
    printf("Result: Correct=%5d  Incorrect=%5d",imgCount+1-errCount,errCount);
    printf("  Accuracy=%5.4f%% \n", a*100);
    
}

void displayTestingProgress(int imgCount, int errCount, int y, int x)
{
    
    double p = (double)(imgCount+1)/(double)(MNIST_MAX_TESTING_IMAGES)*100;
    
    if (x!=0 && y!=0) locateCursor(y, x);
    
    printf("2: TESTING:  Reading image No. %5d",(imgCount+1));
    printf(" of %5d images [%3d%%]  ", MNIST_MAX_TESTING_IMAGES,(int)p);
    
    
    double a = 1 - ((double)errCount/(double)(imgCount+1));
    
    printf("Result: Correct=%5d  Incorrect=%5d",imgCount+1-errCount,errCount);
    printf("  Accuracy=%5.4f%% \n", a*100);
    
}