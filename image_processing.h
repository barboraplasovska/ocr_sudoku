#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include "SDL/SDL_image.h"
#include "SDL/SDL.h"
void init_sdl();
SDL_Surface* load_image(char *path);
Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);
void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);
void ApplyGreyscale(SDL_Surface *surface);
void ApplyBlackAndWhite(SDL_Surface *surface, Uint32 threshold);
SDL_Surface* ApplyBlackAndWhiteAdaptiveMedian(SDL_Surface *surface,
		int filter_size, int C);
SDL_Surface* ApplyMeanFilter(SDL_Surface *surface, int filter_size);
void InvertColors(SDL_Surface *surface);
SDL_Surface* RotateSurface(SDL_Surface *surface, double angleInDegrees);
void EnhanceSurfaceContrast(SDL_Surface *surface, int C);
int** FileToMatrix(char path[]);
void SaveSolvedGrid(int** oldgrid, int** grid,char path[]);
void loop1 (SDL_Surface *surface,
    int *yt, int *middlet, int *failt, int *possiblet, int *i2t);
void loop2 (SDL_Surface *surface,
    int *yt, int *middlet, int *failt, int *possiblet, int *i2t);
void loop3 (SDL_Surface *surface, int *yt, int *middlet, int *failt,
    int *possiblet, int *i2t);
void loop4 (SDL_Surface *surface, int *yt, int *middlet, int *failt,
    int *possiblet, int *i2t);
int mod(SDL_Surface *surface, SDL_Surface *img, int x, int y,int t,int tt);
void Getboxes (SDL_Surface *surface, int x1, int y1, int x2, int y2,
    int x3, int y3, int x4, int y4, char name[]);
SDL_Surface* FindCorners(SDL_Surface *surface, char name[], int foundCorners,
		int getBoxes);
void ApplyAllFilters(SDL_Surface *surface, char name[]);
void CreateFolder(char* folderName);

#endif
