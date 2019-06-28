#include "creature.h"
#include <iostream>

void Creature::SetTexture(const char* filename)
{
	texture = IMG_LoadTexture(*context, filename);
	if (texture == NULL)
	{
		printf("IMG_LoadTexture Error: %s\n", IMG_GetError() );
		exit(1);
	}	
}                

void Creature::UpdatePosition(int wx0, int wy0) {

	int xcol = 0,ycol = 0;
	
///////////////////////////////////////
// collision check here

	objs->Collisions(wx0,wy0,locx,locy,w,h,&x_vel,&y_vel,&xcol,&ycol);	

	if(x_vel)
		locx += x_vel;
	if(xcol)
		x_vel = 0;
	

	if(y_vel)
		locy += y_vel;

	if(ycol) {
		y_vel = 0;
		jump_on = 0;
		fall_on = ydir <=0;
		air_time = 0;
		//state = STATE_STAND;	
	}
	
	dst.x = locx - wx0;
	dst.y = locy - wy0;

//	dst.x = 300;
	//dst.y = 220;
}



/*/
//
// Author:		Daniel Foix
// Function:	BroadCollision()
// Accepts:		Pointer to the shape to check for a collision (Creature *)
// Returns:		Nothing
// Purpose:		Checks if an AABB collision occured between the shapes
// Modified:	09/30/2018
//
int Creature::BroadCollision(Creature * other){

	// calculating AABB projections exist on the coordinate axises
    int d1x = (x0-w/2) - (other->x0+other->w/2);
    int d1y = (y0+w/2) - (other->y0-other->w/2);
    int d2x = (other->x0-other->w/2) - (x0+w/2);
    int d2y = (other->y0+other->w/2) - (y0-w/2);

// drawing the AABB of both shapes, if debug mode is on
#ifdef DEBUG	
	SDL_SetRenderDrawColor(*context, R, G, B, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(*(other->context), (other->x0+other->w/2), (other->y0+other->w/2) ,
							           	  (other->x0+other->w/2), (other->y0-other->w/2));
	SDL_RenderDrawLine(*(other->context), (other->x0-other->w/2), (other->y0+other->w/2) ,
								          (other->x0-other->w/2), (other->y0-other->w/2));
	SDL_RenderDrawLine(*(other->context), (other->x0+other->w/2), (other->y0-other->w/2) ,
								          (other->x0-other->w/2), (other->y0-other->w/2));								 
	SDL_RenderDrawLine(*(other->context), (other->x0+other->w/2), (other->y0+other->w/2) ,
										  (other->x0-other->w/2), (other->y0+other->w/2));										 
	SDL_RenderDrawLine(*(context), (x0+w/2), (y0+w/2) ,
							  (x0+w/2), (y0-w/2));
	SDL_RenderDrawLine(*(context), (x0-w/2), (y0+w/2) ,
							  (x0-w/2), (y0-w/2));
	SDL_RenderDrawLine(*(context), (x0+w/2), (y0-w/2) ,
							  (x0-w/2), (y0-w/2));								 
	SDL_RenderDrawLine(*(context), (x0+w/2), (y0+w/2) ,
							  (x0-w/2), (y0+w/2));
#endif

	// if any projections do not intersect, no collisions are present
    if (d1x > 0 || d1y < 0)
        return 0;

    if (d2x > 0 || d2y < 0)
        return 0;

    return 1;
}
//*/
