////////////////////////////////////////////////////////////////////////
/////
/////	Author:		Daniel Foix
/////	Class:		Shape
/////	Purpose:	Parent class on drawing/tracking shapes on window
/////	Modified	09/30/2018
/////
////////////////////////////////////////////////////////////////////////

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../objects/objectmanager.h"
//#define DEBUG

#ifndef CREATURE_H
#define CREATURE_H

using namespace std; 

class Creature
{
    // Access specifier 
    public: 
	void SetTexture(const char* filename);
	virtual void CalcVelocity() {};
	void UpdatePosition(int wx0, int wy0);
	virtual void UpdateFrame() {};
	void Hurt() {};
	inline float UpdateYVel(float t, int speed){

	float decent = jump_on ? GRAV_ACCEL*(t/1000)*(t/1000) - speed*t/1000 
						   : GRAV_ACCEL*(t/1000)*(t/1000);

	if(jump_on)
		return decent > TERMINAL_VEL ? 		
			TERMINAL_VEL : decent;	
	if(fall_on)	
		return decent > TERMINAL_VEL ? 		
			TERMINAL_VEL : decent; 	

	}
//	int BroadCollision(Shape * other);
	
	// upper left coordinates of the shape
	int x0 = 0, y0 = 0;
	// side length of AABB for the shape
	int w, h;

	// renderer for shape drawing
	ObjectManager * objs;
	SDL_Renderer** context;
	SDL_Texture* texture;
	//SDL_Rect src;
	SDL_Rect dst;

	//window dimensions
	int winx = 0,winy= 0;
	
	// movement
	float x_vel = 0, y_vel = 0;
	int locx = 0, locy = 0;

	int xdir = 0;
	int ydir = 0;
	int fall_on = 0;
	int jump_on = 0;
	int state = 0;
	int air_time = 0;

	// animation variables
	int pose_cur;
	int pose_num;
	int pose_timer;
	int pose_length;

	// stats
	int health_max = 100;
	int health_cur = health_max;
}; 

#endif
