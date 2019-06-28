/*
 * slime.h
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


#ifndef SLIME_H
#define SLIME_H

#include "monster.h"
#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "enemymanager.h"

class Slime: public Monster
{
	public:
		Slime(SDL_Renderer** renderer, ObjectManager * stuff, int x, int y, int nw, int nh, int health);
		void SetTexture(const char* filename);
		int IsActive(int wx0, int wy0, int gx, int gy);
		void CalcVelocity(int gx, int gy);
		void UpdatePosition(int wx0, int wy0, int gx, int gy,int gw,int gh,SDL_Rect * src);
		void UpdateFrame(SDL_Rect * src);
		
};

#endif /* SLIME_H */ 
