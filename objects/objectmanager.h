/*
 * objectmanager.h
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


#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <iostream>
#include "objm_config.h"
#include "object.h"
#include "tile.h"
#include "collision_help.h"
#include "../levels/game_config.h"
#include "../backgrounds/background.h"
#include "../structures/ll.h"

#define OBJECT_SCALE 1

typedef void (*collision_ptr)( int, int, int, int, 
									  int, int, int, int, 
									  float *, float *, 
									  int *, int * );

class ObjectManager
{
	public:
		ObjectManager(SDL_Renderer** renderer);
		void LoadTiles();
		int ReadLevelTiles(int * px, int * py, int adv, char * sound, const char * level_config, const char * background_file, const char * midground_file, const char * foreground_file);
		void DrawBackgroundTiles(int wx0, int wy0);
		void DrawMidgroundTiles(int wx0, int wy0);
		void DrawForegroundTiles(int wx0, int wy0);
		void Collisions(int wx0, int wy0, int px0, int py0, int pw0, int ph0,  float * xvel, float * yvel, int * xcol, int * ycol);
		void CollisionTiles(int wx0, int wy0, int px0, int py0, int pw0, int ph0, float * xvel, float * yvel, int * xcol, int * ycol);
		void DeleteObjects();
		void PrintTiles();
		LL collision_tiles;
		int level_x = 0, level_y = 0;
		collision_ptr collision_types[4] = { Collision0,
											 Collision1,
											 Collision2,
											 Collision3};
	private:
		LL tangible_tiles, background_tiles, foreground_tiles;
		SDL_Renderer** context;		
		SDL_Texture* source_tiles_textures[MAXTILES];
		SDL_Rect source_tiles_rect[MAXTILES];
		int collision_codes[MAXTILES];
		/* add your private declarations */
};

#endif /* OBJECTMANAGER_H */ 
