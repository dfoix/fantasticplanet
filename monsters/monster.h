/*
 * monster.h
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


#ifndef MONSTER_H
#define MONSTER_H

#include "../creatures/creature.h"
//#include "enemy_config.h"


class Monster: public Creature
{
	
//	typedef void (Method::*method_function)(int, int, int, int);
//	method_function method_pointer[4] = {&Method::add, &Method::subtract,  &Method::multiply, &Method::divide};
//	method_function func = method_pointer[method_type];
//	result = (this->*func)(x,y);
	
	typedef void (Monster::*update_ptr)(int wx0, int wy0, int gx, int gy);

	public:
		int intersection = 0;
		
		void UpdateSlime(int wx0, int wy0, int gx, int gy) {}
		void UpdateFace(int wx0, int wy0, int gx, int gy) {}
		
		// MAXENEMIES -> 2
		update_ptr update_types[2] = {&Monster::UpdateSlime,&Monster::UpdateFace};
			
	private:
		/* add your private declarations */
};

#endif /* MONSTER_H */ 
