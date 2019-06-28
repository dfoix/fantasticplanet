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
//#define DEBUG

#ifndef OBJECT_H
#define OBJECT_H

using namespace std; 

class Object
{
    // Access specifier 
    public: 
    virtual void Update(){};
	virtual void DrawShape(){};
	virtual void NarrowCollision() {};	
//	int BroadCollision(Shape * other);
	void MoveCenter();
	
	void FillShape();
	
	// center coordinates of the shape
	int x0 = 0, y0 = 0;
	// side length of AABB for the shape
	int w = 10;

	// renderer for shape drawing	
	SDL_Rect dst;

	//window dimensions
	int winx = 0,winy= 0;
}; 

#endif
