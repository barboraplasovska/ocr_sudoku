#include <stdlib.h>
#include <string.h>
#include "mnist-utils.h"

uint32_t flipBytes(uint32_t n)
{    
    uint32_t b0,b1,b2,b3;
    b0 = (n & 0x000000ff) <<  24u;
    b1 = (n & 0x0000ff00) <<   8u;
    b2 = (n & 0x00ff0000) >>   8u;
    b3 = (n & 0xff000000) >>  24u;
    return (b0 | b1 | b2 | b3);
}

void readImageFileHeader(FILE *imageFile, MNIST_ImageFileHeader *ifh)
{    
    ifh->magicNumber =0;
    ifh->maxImages   =0;
    ifh->imgWidth    =0;
    ifh->imgHeight   =0;
    
    fread(&ifh->magicNumber, 4, 1, imageFile);
    ifh->magicNumber = flipBytes(ifh->magicNumber);
    fread(&ifh->maxImages, 4, 1, imageFile);
    ifh->maxImages = flipBytes(ifh->maxImages);
    fread(&ifh->imgWidth, 4, 1, imageFile);
    ifh->imgWidth = flipBytes(ifh->imgWidth);
    fread(&ifh->imgHeight, 4, 1, imageFile);
    ifh->imgHeight = flipBytes(ifh->imgHeight);
}

void readLabelFileHeader(FILE *imageFile, MNIST_LabelFileHeader *lfh)
{    
    lfh->magicNumber =0;
    lfh->maxImages   =0;
    fread(&lfh->magicNumber, 4, 1, imageFile);
    lfh->magicNumber = flipBytes(lfh->magicNumber);
    fread(&lfh->maxImages, 4, 1, imageFile);
    lfh->maxImages = flipBytes(lfh->maxImages);
}

FILE *openMNISTImageFile(char *fileName)
{
    FILE *imageFile;
    imageFile = fopen (fileName, "rb");
    if (imageFile == NULL) 
    {
        printf("Abort! Could not fine MNIST IMAGE file: %s\n",fileName);
        exit(0);
    }
    MNIST_ImageFileHeader imageFileHeader;
    readImageFileHeader(imageFile, &imageFileHeader);
    return imageFile;
}

FILE *openMNISTLabelFile(char *fileName)
{    
    FILE *labelFile;
    labelFile = fopen (fileName, "rb");
    if (labelFile == NULL) 
    {
        printf("Abort! Could not find MNIST LABEL file: %s\n",fileName);
        exit(0);
    }
    MNIST_LabelFileHeader labelFileHeader;
    readLabelFileHeader(labelFile, &labelFileHeader);
    return labelFile;
}

MNIST_Image getImage(FILE *imageFile)
{
    MNIST_Image img;
    size_t result;
    result = fread(&img, sizeof(img), 1, imageFile);
    if (result!=1) 
    {
        printf("\nError when reading IMAGE file! Abort!\n");
        exit(1);
    }
    return img;
}

MNIST_Label getLabel(FILE *labelFile)
{
    MNIST_Label lbl;
    size_t result;
    result = fread(&lbl, sizeof(lbl), 1, labelFile);
    if (result!=1) 
    {
        printf("\nError when reading LABEL file! Abort!\n");
        exit(1);
    }
    return lbl;
}