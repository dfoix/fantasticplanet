/*
 * weapon.hpp
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


#ifndef WEAPON_HPP
#define WEAPON_HPP

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"


class Weapon
{
	public:
		Weapon();
		void DrawShape(SDL_Renderer** renderer, SDL_Rect * source, SDL_Texture ** texture, int wx0, int wy0);
		int type = 0;
		int active = 0;
		int xdir = 0;
		int ydir = 0;
		int distance = 0;
		SDL_Rect dst;		
		int x0 = 0, y0 = 0;
		
		/* add your private declarations */
};

#endif /* WEAPON_HPP */ 
