#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "object.h"
#include "../player/player_config.h" 
#include "../levels/game_config.h"

#ifndef TILE_H
#define TILE_H
//#define C_TEST



using namespace std; 

class Tile : public Object
{
    // Access specifier 
    public: 
    Tile(int nT, int xN0, int yN0, int dw, int dh,float scale);
	void DefineTile(int nT, int xN0, int yN0, int dw, int dh);
	void DrawShape(SDL_Renderer** renderer, SDL_Rect * source, SDL_Texture ** texture, int wx0, int wy0);
	int OnScreen(int wx0, int wy0);
	int OnCollsion(int wx0, int wy0);	
	int NarrowCollision(int px0, int py0, int pw0, int ph0);	
	void PrintParameters();
	void MoveCenter();

	// coordinates for the shape corners
	int xl = 0, xr = 0;
	int yt = 0, yb = 0;	
	
	// side length of AABB for the shape
	int w = 10;
	
	int type = 0;
	//window dimensions
	int winx = 0,winy= 0;
}; 

#endif
