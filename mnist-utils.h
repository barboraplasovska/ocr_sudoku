#ifndef MNIST_UTILS_H
#define MNIST_UTILS_H

#include <stdint.h>
#include <stdio.h>

#define MNIST_TRAINING_SET_IMAGE_FILE_NAME "data/train-images-idx3-ubyte"
#define MNIST_TRAINING_SET_LABEL_FILE_NAME "data/train-labels-idx1-ubyte"
#define MNIST_TESTING_SET_IMAGE_FILE_NAME "data/t10k-images-idx3-ubyte"
#define MNIST_TESTING_SET_LABEL_FILE_NAME "data/t10k-labels-idx1-ubyte"
#define MNIST_MAX_TRAINING_IMAGES 60000
#define MNIST_MAX_TESTING_IMAGES 10000
#define MNIST_IMG_WIDTH 28
#define MNIST_IMG_HEIGHT 28

typedef struct MNIST_ImageFileHeader MNIST_ImageFileHeader;
typedef struct MNIST_LabelFileHeader MNIST_LabelFileHeader;

typedef struct MNIST_Image MNIST_Image;
typedef uint8_t MNIST_Label;

struct MNIST_Image{
    uint8_t pixel[MNIST_IMG_WIDTH*MNIST_IMG_HEIGHT];
};

struct MNIST_ImageFileHeader{
    uint32_t magicNumber;
    uint32_t maxImages;
    uint32_t imgWidth;
    uint32_t imgHeight;
};

struct MNIST_LabelFileHeader{
    uint32_t magicNumber;
    uint32_t maxImages;
};

FILE *openMNISTImageFile(char *fileName);
FILE *openMNISTLabelFile(char *fileName);
MNIST_Image getImage(FILE *imageFile);
MNIST_Label getLabel(FILE *labelFile);
int getStandardDigitPixel(int num, int pixelId);

#endif