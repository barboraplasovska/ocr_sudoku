#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "image_processing.h"

#include <math.h>


int main()
{
	//Initialize the variables
	SDL_Surface* image_surface;
	
	//Initialize the SDL
	init_sdl();
	
	//load the image
	image_surface = load_image("image_02.jpeg");
	//image_surface = RotateSurface(image_surface, 40);
	
	//Rotate(image_surface, 40);
	
	ApplyGreyscale(image_surface);
	ApplyBlackAndWhite(image_surface);
	FindCorners(image_surface);
	SDL_SaveBMP(image_surface, "explainedDetection.jpeg");
	
	/* if(SDL_SaveBMP(image_surface, "blackandwhite.bmp") != 0)
	{
	 	// Error saving bitmap
	 	printf("SDL_SaveBMP failed: %s\n", SDL_GetError());
        } */
        
        //SDL_RenderCopyEx();
        
        
        SDL_SaveBMP(image_surface, "test.bmp");
        
        SDL_FreeSurface(image_surface);
        
	return 0;
}
