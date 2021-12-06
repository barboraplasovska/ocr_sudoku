#ifndef MNIST_STATS_H
#define MNIST_STATS_H

#include "mnist-utils.h"

void displayImageFrame(int y, int x);
void displayImage(MNIST_Image *img, int lbl, int cls, int row, int col);
void displayTrainingProgress(size_t epoch, int imgCount, int errCount, int y, int x);
void displayTestingProgress(int imgCount, int errCount, int y, int x);

#endif