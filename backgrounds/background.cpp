#include "background.h"
#include <iostream>

Background::Background(SDL_Renderer** renderer){
	context = renderer;
}

void Background::SetWX(int oX){
	wx0 = oX;
}

void Background::SetWY(int oY){
	wy0 = oY;
}

void Background::RefreshScreen(){
	if(!active)
		return;
		
	int xoff =  (wx0%WINDOW_X);
	int yoff =   wy0%WINDOW_Y;

	xoff = xoff >=0 ? xoff : -xoff;
	yoff = yoff >=0 ? -yoff : yoff;
	bac_dst.x = mid_dst.x = xoff;
	bac_dst.y = mid_dst.y = yoff;
		
	SDL_RenderCopy(*context, back, &bac_src, &bac_dst);
	SDL_RenderCopy(*context, mid, &mid_src, &mid_dst);

	bac_dst.x = mid_dst.x = xoff - WINDOW_X;
	bac_dst.y = mid_dst.y = yoff + WINDOW_Y;
		
	SDL_RenderCopy(*context, back, &bac_src, &bac_dst);
	SDL_RenderCopy(*context, mid, &mid_src, &mid_dst);

	bac_dst.x = mid_dst.x = xoff;
	bac_dst.y = mid_dst.y = yoff + WINDOW_Y;
		
	SDL_RenderCopy(*context, back, &bac_src, &bac_dst);
	SDL_RenderCopy(*context, mid, &mid_src, &mid_dst);
	
	bac_dst.x = mid_dst.x = xoff - WINDOW_X;
	bac_dst.y = mid_dst.y = yoff;
		
	SDL_RenderCopy(*context, back, &bac_src, &bac_dst);
	SDL_RenderCopy(*context, mid, &mid_src, &mid_dst);
	
}

void Background::SetTextures(const char* background_file, const char* midground_file,
							 int back_w, int back_h, int mid_w, int mid_h)
{
	back = IMG_LoadTexture(*context, background_file);
	mid = IMG_LoadTexture(*context, midground_file);
	if (!(back && mid))
	{
		printf("IMG_LoadTexture Error: %s\n", IMG_GetError() );
		exit(1);
	}	

	bac_dst.x = 0; bac_dst.y = 0; bac_dst.w = WINDOW_X; bac_dst.h = WINDOW_Y;
	bac_src.x = 0; bac_src.y = 0; bac_src.w = back_w; bac_src.h = back_h;

	mid_dst.x = 0; mid_dst.y = 0; mid_dst.w = WINDOW_X; mid_dst.h = WINDOW_Y;
	mid_src.x = 0; mid_src.y = 0; mid_src.w = mid_w; mid_src.h = mid_h;
}                
