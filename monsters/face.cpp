/*
 * face.cpp
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


#include "face.h"


Face::Face(SDL_Renderer** renderer, ObjectManager * stuff, int x, int y, int nw, int nh, int health)
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

void Face::SetTexture(const char* filename) {

}
int Face::IsActive(int wx0, int wy0, int gx, int gy) {

		// returns if face is onscreen. update later for more complex activity
	return (wx0 - WINDOW_X/4 < locx + w  &&
			wx0 + WINDOW_X + WINDOW_X/4 > locx  &&
			wy0 - WINDOW_Y/4 < locy + h &&
			wy0 + WINDOW_Y + WINDOW_Y/4 > locy);
}

int Face::UpdatePosition(int wx0, int wy0, int gx, int gy,int gw,int gh,SDL_Rect * src) {
	CalcVelocity(gx,gy);
	Creature::UpdatePosition(wx0,wy0);
	UpdateFrame(src);
	return DamagePlayer(gx,gy,gw,gh);
}

int Face::DamagePlayer(int px, int py, int pw, int ph){
	if((px < locx + w &&
			px + pw > locx &&
			py < locy + h &&
			py + ph > locy))
		return FACEDAMAGE;
	else
		return 0;
}


void Face::UpdateFrame(SDL_Rect * src) {
	src->x = src->w*xdir;
}



void Face::CalcVelocity(int gx, int gy)
{

	if (gx > locx) {
		x_vel = FACEVEL_X;
		xdir = 0;
	}
	if (locx+w>gx+GD_W) {
		x_vel = -FACEVEL_X;
		xdir = 1;
	}
	if (gx > locx) {
		x_vel = FACEVEL_X;
		xdir = 0;
	}
	if (gy>locy)
		y_vel = FACEVEL_Y;
	if (locy+h>gy+GD_H)
		y_vel = -FACEVEL_Y;
}


