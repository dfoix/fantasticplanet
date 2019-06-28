/*
 * face.h
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


#ifndef JUMPER_H
#define JUMPER_H

#include "monster.h"
#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../player/player_config.h"
#include "enemymanager.h"

class Jumper : public Monster
{
	public:
		Jumper(SDL_Renderer** renderer, ObjectManager * stuff, int x, int y, int nw, int nh, int health);
		void SetTexture(const char* filename);
		int IsActive(int wx0, int wy0, int gx, int gy);
		void CalcVelocity(int gx, int gy);
		int UpdatePosition(int wx0, int wy0, int gx, int gy,int gw,int gh,SDL_Rect * src);
		void UpdateFrame(SDL_Rect * src);
		int DamagePlayer(int px, int py, int pw, int ph);


		/* add your private declarations */
		
};

#endif /* FACE_H */ 
