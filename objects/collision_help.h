#ifndef COLLISIONS_H
#define COLLISIONS_H

#include "../levels/game_config.h"
#include <math.h>
#define PI 3.141592653589793238462643383279502884L
// Given three colinear points p, q, r, the function checks if 
// point q lies on line segment 'pr' 
inline bool doIntersect(float x1, float y1, float x2, float y2,
				  float x3, float y3, float x4, float y4) {
	// Store the values for fast access and easy
	// equations-to-code conversion

	float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
	// If d is zero, there is no intersection
	if (d == 0) return NULL;

	// Get the x and y
	float pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
	float x = ( pre * (x3 - x4) - (x1 - x2) * post ) / d;
	float y = ( pre * (y3 - y4) - (y1 - y2) * post ) / d;

	// Check if the x and y coordinates are within both lines
	if ( x < min(x1, x2) || x > max(x1, x2) ||
		x < min(x3, x4) || x > max(x3, x4) ) return 0;
	if ( y < min(y1, y2) || y > max(y1, y2) ||
		y < min(y3, y4) || y > max(y3, y4) ) return 0;

	return 1;
}

inline bool RectangleLineIntersection(int px0, int py0, int pw0, int ph0,
									  int p1x, int p1y, int q1x, int q1y){
	return (doIntersect(px0, py0, 
						px0+pw0, py0, 
						p1x, p1y, 
						q1x, q1y) ||
		 doIntersect(px0+pw0, py0, 
					 px0+pw0, py0+ph0, 
					 p1x, p1y, 
					 q1x, q1y) ||
		 doIntersect(px0+pw0, py0+ph0, 
					 px0, py0+ph0, 
					 p1x, p1y, 
					 q1x, q1y) ||
		 doIntersect(px0, py0+ph0, 
					 px0, py0, 
					 p1x, p1y, 
					 q1x, q1y));
}


inline void Collision0(int xl, int xr, int yt, int yb, 
				int px0, int py0, int pw0, int ph0, 
				float * xvel, float * yvel, 
				int * xcol, int * ycol){

	if (!((px0+*xvel) < xr &&
			(px0+*xvel) + pw0 > xl &&
			(py0+*yvel) < yb &&
			(py0+*yvel) + ph0 > yt))
			return;

	if((px0+*xvel) < xr && 
	   (px0+*xvel) + pw0 > xl && 
	   (py0) < yb && 
	   (py0) + ph0 > yt){
		*xcol = 1;
		if(*xvel > 0){					
					*xvel = xl - (px0 + pw0);
			}
		else if(*xvel < 0){
				*xcol = 1;
				*xvel = px0 -xr;
			}
	}
		
	if ((px0) < xr &&
		(px0) + pw0 > xl &&
		(py0+*yvel) < yb &&
		(py0+*yvel) + ph0 > yt){
		*ycol = 1;
		if(*yvel > 0){

				*yvel = yt - (py0 + ph0);
		}
		else if(*yvel < 0){

				*yvel = yb - py0;
		}
	}
		
	return;	
}

inline void Collision1(int xl, int xr, int yt, int yb, 
				int px0, int py0, int pw0, int ph0, 
				float * xvel, float * yvel, 
				int * xcol, int * ycol){
	
	int offset = (yb - yt)/2;

	if (!((px0+*xvel) < xr &&
			(px0+*xvel) + pw0 > xl &&
			(py0+*yvel) < yb &&
			(py0+*yvel) + ph0 > yt + offset))
			return;

	if((px0+*xvel) < xr && 
	   (px0+*xvel) + pw0 > xl && 
	   (py0) < yb && 
	   (py0) + ph0 > yt + offset){
		*xcol = 1;
		if(*xvel > 0){					
					*xvel = xl - (px0 + pw0);
			}
		else if(*xvel < 0){
				*xcol = 1;
				*xvel = px0 -xr;
			}
	}
		
	if ((px0) < xr &&
		(px0) + pw0 > xl &&
		(py0+*yvel) < yb &&
		(py0+*yvel) + ph0 > yt + offset){
		*ycol = 1;
		if(*yvel > 0){

				*yvel = yt + offset - (py0 + ph0);
		}
		else if(*yvel < 0){

				*yvel = yb - py0;
		}
	}
		
	return;	
}

inline void Collision2(int xl, int xr, int yt, int yb, 
				int px0, int py0, int pw0, int ph0, 
				float * xvel, float * yvel, 
				int * xcol, int * ycol){
					
	if (!((px0+*xvel) < xr &&
			(px0+*xvel) + pw0 > xl &&
			(py0+*yvel) < yb &&
			(py0+*yvel) + ph0 > yt))
			return;
			
	if(!(RectangleLineIntersection(px0+*xvel,py0+*yvel-1,pw0,ph0,xl,yb,xr,yt)))
		return;
	
	if(RectangleLineIntersection(px0+*xvel,py0-1,pw0,ph0,xl,yb,xr,yt)){
		*xcol = 1;
/*		if(*yvel >= 0 && *xvel >= 0){					
			*yvel = *xvel*cos(PI/4.0) + 1;			
			*ycol = 1;
		}
		if(*yvel >= 0 && *xvel < 0){					
			*yvel = 1-*xvel*cos(PI/4.0);			
			*ycol = 1;
		}

		else {
*/			*xvel = 0;
		
//		}
	}
		
	if(RectangleLineIntersection(px0,py0+*yvel-1,pw0,ph0,xl,yb,xr,yt)){
		*ycol = 1;
		if(*yvel > 0){
				*yvel = yt - (py0 + ph0);
		}
		else if(*yvel < 0){
				*yvel = yb - py0;
		}
		*yvel = 0;
	}
		
	return;	
}

inline void Collision3(int xl, int xr, int yt, int yb, 
				int px0, int py0, int pw0, int ph0, 
				float * xvel, float * yvel, 
				int * xcol, int * ycol){

	if (!((px0+*xvel) < xr &&
			(px0+*xvel) + pw0 > xl &&
			(py0+*yvel) < yb &&
			(py0+*yvel) + ph0 > yt))
			return;
			
	if(!(RectangleLineIntersection(px0+*xvel,py0+*yvel-1,pw0,ph0,xr,yb,xl,yt)))
		return;
	
	if(RectangleLineIntersection(px0+*xvel,py0-1,pw0,ph0,xr,yb,xl,yt)){
		*xcol = 1;
/*		if(*yvel >= 0 && *xvel >= 0){					
			*yvel = *xvel*cos(PI/4.0) + 1;			
			*ycol = 1;
		}
		if(*yvel >= 0 && *xvel < 0){					
			*yvel = 1-*xvel*cos(PI/4.0);			
			*ycol = 1;
		}

		else {
*/			*xvel = 0;
		
//		}
	}
		
	if(RectangleLineIntersection(px0,py0+*yvel-1,pw0,ph0,xr,yb,xl,yt)){
		*ycol = 1;
		if(*yvel > 0){
				*yvel = yt - (py0 + ph0);
		}
		else if(*yvel < 0){
				*yvel = yb - py0;
		}
		*yvel = 0;
	}
		
	return;	
}

typedef void (*collision_ptr)( int, int, int, int, 
									  int, int, int, int, 
									  float *, float *, 
									  int *, int * );
#endif
