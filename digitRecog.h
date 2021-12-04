#ifndef DIGITRECOG_H
#define DIGITRECOG_H

#include "network.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL.h"

int findDigit(Network *nn, SDL_Surface *image);
Network* setupNetwork(void);
void shutDownNetwork(Network *nn);
void loadWeights(char *filePath, Network *nn);
void saveWeights(Network *nn);

#endif
