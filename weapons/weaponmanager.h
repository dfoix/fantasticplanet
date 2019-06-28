/*
 * weaponmanager.hpp
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

#ifndef WEAPONMANAGER_HPP
#define WEAPONMANAGER_HPP

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "weapm_config.h"
#include "../levels/game_config.h"
#include "../objects/object.h"
#include "../objects/tile.h"
#include "../creatures/creature.h"
#include "../backgrounds/background.h"
#include "../structures/ll.h"
#include "../Fixed_print.h"
#include "weapon.h"

class WeaponManager
{
	public:
		WeaponManager(SDL_Renderer** renderer, ObjectManager *);
		void ReadWeapons(float shot_scale);
		void UpdateWeapons(int wx0, int wy0);
		void UpdateVelocities();
		Node * AvailibleShot(int t);
		void DrawWeapons(int wx0, int wy0);
		void ObjectCollisions(int wx0, int wy0);
		void EnemyCollision(int wx0, int wy0, Creature *,int r, int g, int b);
		void FireWeapons(int active,int x0, int y0, int xdir, int ydir, int wx0, int wy0);
		void DeleteWeapons();
		
	private:
		ObjectManager * objs;
		LL weaps[MAXWEAPONS];
		LL actives;
		char string[6];
		SDL_Color color;

		// printing the shot statistics header
		int stats[MAXWEAPONS][5]; // for each type: speed, damage, width, height, distance
		SDL_Renderer** context;		
		SDL_Texture* source_weapons_textures[MAXWEAPONS][3];
		SDL_Rect source_weapons_rect[MAXWEAPONS][2];
		SDL_Rect initial_weapons_dst[MAXWEAPONS];
		Mix_Chunk * weapon_sounds[MAXWEAPONS];
		Mix_Chunk * weapon_hits[MAXWEAPONS];
};

#endif /* WEAPONMANAGER_HPP */ 
