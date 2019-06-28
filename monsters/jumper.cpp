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


Jumper::Jumper(SDL_Renderer** renderer, ObjectManager * stuff, int x, int y, int nw, int nh, int health)
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
	pose_num = 5;
}

void Jumper::SetTexture(const char* filename) {

}
int Jumper::IsActive(int wx0, int wy0, int gx, int gy) {

		// returns if face is onscreen. update later for more complex activity

	return (wx0 - WINDOW_X/4 < locx + w  &&
			wx0 + WINDOW_X + WINDOW_X/4 > locx  &&
			wy0 - WINDOW_Y/4 < locy + h &&
			wy0 + WINDOW_Y + WINDOW_Y/4 > locy);

}

int Jumper::UpdatePosition(int wx0, int wy0, int gx, int gy,int gw,int gh,SDL_Rect * src) {
	CalcVelocity(gx,gy);
	Creature::UpdatePosition(wx0,wy0);
	UpdateFrame(src);
	return DamagePlayer(gx,gy,gw,gh);
}

int Jumper::DamagePlayer(int px, int py, int pw, int ph){
	if((px < locx + w &&
			px + pw > locx &&
			py < locy + h &&
			py + ph > locy))
		return JUMPER_DAMAGE;
	else
		return 0;
}


void Jumper::UpdateFrame(SDL_Rect * src) {
	static int i = 0;

	if(!(jump_on || fall_on)){
		if(i++ > JUMPER_DELAY){
			i = 0;
			pose_cur = (pose_cur+1)%pose_num;		
			src->x = pose_cur*src->w;
		}
	}
	src->x = pose_cur*src->w;
}



void Jumper::CalcVelocity(int gx, int gy)
{
	
	switch(fall_on || jump_on)
	{
	
	case true:		
		air_time += SDL_DELAY;		
		y_vel = UpdateYVel((float)air_time,jump_on ? JUMP_SPEED : 0);
		ydir = y_vel >= 0;
		pose_cur *= (1-ydir);
		break;

	case false:
			if(pose_cur < 4)
			{
				fall_on = 1;
				y_vel = UpdateYVel(SDL_DELAY,JUMP_SPEED);
			}
			else
			{
				air_time = 0;
				jump_on = 1;
			}
			break;
		}
	
}


