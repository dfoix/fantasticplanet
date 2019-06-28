/*
 * enemy_manager.h
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


#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "monster.h"
#include "enemy_config.h"
#include "../player/player.h"
#include "../objects/objectmanager.h"
#include "../weapons/weaponmanager.h"
#include "../levels/game_config.h"

class EnemyManager
{
	typedef void (Monster::*update_ptr)(int wx0, int wy0, int gx, int gy);

	public:
		EnemyManager(SDL_Renderer** renderer, Player * theDude, ObjectManager * theStuff,WeaponManager * boom);			
		void ReadEnemies();
		int UpdateEnemies(int wx0, int wy0, int gx, int gy);
		void UpdateEnemyList(const char * level_file);
		void DeleteEnemies();
		void DrawEnemies(int wx0, int wy0, int gx, int gy);
		LL level_enemies;

	private:
		Player * dude;
		int stats[MAXENEMIES][3]; // w, h, health
		ObjectManager * objs;
		WeaponManager * weaps;
		SDL_Renderer** context;		
		SDL_Texture* source_enemy_textures[MAXENEMIES];
		SDL_Rect source_enemy_rect[MAXENEMIES];
		
};

#endif /* ENEMY_MANAGER_H */ 
