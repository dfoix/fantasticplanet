/*
 * weapon.cpp
 * 
 * Copyright 2018 d <deimos@mercury>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include "weapon.h"


Weapon::Weapon()
{
	
}

void Weapon::DrawShape(SDL_Renderer** renderer, SDL_Rect * source, SDL_Texture ** texture, int wx0, int wy0){

	dst.x = x0 - wx0;
	dst.y = y0 - wy0;

#ifdef C_TEST
	int R = 255, G = 0, B = 0;
	// draws four lines to create a square centered at x0/y0
	SDL_SetRenderDrawColor(*renderer, R, G, B, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(*renderer, xl, yt ,
								 xr, yt);
	SDL_RenderDrawLine(*renderer, xr, yt ,
								 xr, yb);
	SDL_RenderDrawLine(*renderer, xr, yb ,
								 xl, yb);
	SDL_RenderDrawLine(*renderer, xl, yb ,
								 xl, yt);
#endif
								 
	SDL_RenderCopy(*renderer, *texture, source, &dst);
}
