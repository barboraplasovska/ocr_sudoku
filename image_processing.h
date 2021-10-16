#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

void init_sdl();
SDL_Surface* load_image(char *path);
Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);
void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);
void ApplyGreyscale(SDL_Surface *surface);
void ApplyBlackAndWhite(SDL_Surface *surface);
SDL_Surface* RotateSurface(SDL_Surface *surface, int angleInDegrees);
void loop1 (SDL_Surface *surface, int *yt, int *middlet, int *failt, int *possiblet, int *i2t);
void loop2 (SDL_Surface *surface, int *yt, int *middlet, int *failt, int *possiblet, int *i2t);
void loop3 (SDL_Surface *surface, int *yt, int *middlet, int *failt, int *possiblet, int *i2t);
void loop4 (SDL_Surface *surface, int *yt, int *middlet, int *failt, int *possiblet, int *i2t);
void mod(SDL_Surface *surface, SDL_Surface *img, int x, int xx, int y, int yy);
void Getboxes (SDL_Surface *surface, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
void FindCorners(SDL_Surface *surface);

#endif
