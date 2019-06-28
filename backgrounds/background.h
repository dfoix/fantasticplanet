////////////////////////////////////////////////////////////////////////
/////
/////	Author:		Daniel Foix
/////	Class:		Shape
/////	Purpose:	Parent class on drawing/tracking shapes on window
/////	Modified	09/30/2018
/////
////////////////////////////////////////////////////////////////////////

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../levels/game_config.h"

//#define DEBUG

#ifndef BACKGROUND_H
#define BACKGROUND_H

using namespace std; 

class Background
{
    // Access specifier 
    public:
    Background(SDL_Renderer** renderer);
    void SetWX(int oX);
    void SetWY(int oY);
	void RefreshScreen();
	void SetTextures(const char* background_file, const char* midground_file,
					 int back_w, int back_h, int mid_w, int mid_h);
	int active = 0;
	private:
	int wx0 = 0, wy0 = 0;
	
	// renderer for shape drawing
	SDL_Renderer** context;
	SDL_Texture* back;
	SDL_Texture* mid;
	
	SDL_Rect bac_src, bac_dst;
	SDL_Rect mid_src, mid_dst;
}; 

#endif
