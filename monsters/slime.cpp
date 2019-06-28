/*
 * slime.cpp
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


#include "slime.h"


Slime::Slime(SDL_Renderer** renderer, ObjectManager * stuff, int x, int y, int nw, int nh, int health)
{
	context = renderer;
	objs = stuff;
	locx = x;
	locy = y;
	w = nw;
	h = nh;
	dst.w = w;
	dst.h = h;
	health_cur = health_max = health;
	fall_on = jump_on = 0;
}

void Slime::SetTexture(const char* filename) {

}
int Slime::IsActive(int wx0, int wy0, int gx, int gy) {

		// returns if slime is onscreen. update later for more complex activity
	return (wx0 < locx + w &&
			wx0 + WINDOW_X > locx &&
			wy0 < locy + h &&
			wy0 + WINDOW_Y > locy);
}

void Slime::UpdatePosition(int wx0, int wy0, int gx, int gy,int gw, int gh, SDL_Rect * src) {
	CalcVelocity(gx,gy);
	Creature::UpdatePosition(wx0,wy0);
	UpdateFrame(src);
}

void Slime::UpdateFrame(SDL_Rect * src) {
	src->x = 200;
	src->y = 200;
}



void Slime::CalcVelocity(int gx, int gy)
{
	switch(fall_on || jump_on)
	{

	case true:

		if (gx > locx){
			xdir = 0;x_vel = SLIMEVEL_X;}
		if (locx+w>gx+GD_W){
			xdir = 1;x_vel = -SLIMEVEL_X;}
		
		if(air_time > SDL_DELAY) state = STATE_JUMP;
		air_time += SDL_DELAY;
		y_vel = UpdateYVel((float)air_time,30);
		ydir = y_vel >= 0;
		break;

	case false:
//		if(!keyboard[SDL_SCANCODE_W])
//		{
			if (gx > locx) {
				x_vel = SLIMEVEL_X;
				xdir = 0;
				state = STATE_RUN;
			}
			if (locx+w>gx+GD_W) {
				x_vel = -SLIMEVEL_X;
				xdir = 1;
				state = STATE_RUN;
			}
		
//			if(!keyboard[SDL_SCANCODE_D] && !keyboard[SDL_SCANCODE_A] && 
	//		   !keyboard[SDL_SCANCODE_S]){
		//		x_vel = 0;
			//	state = STATE_STAND;
		//	}
			fall_on = 1;
			y_vel = UpdateYVel(SDL_DELAY,30);

/*		}
		else
		{
			air_time = 0;
			jump_on = 1;
			if(x_vel)state = STATE_RUN;
			else if(!x_vel)state = STATE_STAND;
		}
*/
		break;
	}
}

