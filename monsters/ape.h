////////////////////////////////////////////////////////////////////////
/////
/////	Author:		Daniel Foix
/////	Class:		Ape
/////	Purpose:	Draw/track a circle onscreen
/////	Modified	09/30/2018
/////
////////////////////////////////////////////////////////////////////////

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "../player/player_config.h"
#include "../player/player.h"
#include "../backgrounds/background.h"
#include "../objects/objectmanager.h"
#include "../weapons/weaponmanager.h"
#include "../creatures/creature.h"
#include "enemy_config.h"
#include "../levels/game_config.h"

//#define DEBUG

#ifndef APE_H
#define APE_H

#define STATE_STAND 0
#define STATE_RUN 1
#define STATE_CROUCH 2
#define STATE_JUMP 3
#define STATE_HURT 4
//#define C_TEST
//#define DEBUG

using namespace std; 

class Ape : public Creature
{
    // Access specifier 
    public: 
	Ape(SDL_Renderer** renderer, ObjectManager * stuff, int x, int y, int nw, int nh, int health);
	
	inline int UpdateApe(int wx0, int wy0,SDL_Rect * src){
		
		CalcVelocity();
		UpdateFrame(src);
		UpdatePosition(wx0,wy0);
		weaps->UpdateWeapons(wx0,wy0);
		ManageWeapons(wx0,wy0);
		return 1;
	}
	
	void Setup(Player * guy);
	void CalcVelocity();
	int IsActive(int wx0, int wy0, int gx, int gy);
	void ManageWeapons(int wx0, int wy0);
	void UpdatePosition(int wx0, int wy0);
	void UpdateFrame(SDL_Rect * src);
	void ChangeFrames(SDL_Rect * src);
	WeaponManager * weaps;
	
	private:
	Player * dude;
	Mix_Chunk * land_sfx;
	Mix_Chunk * jump_sfx;
	Mix_Chunk * hurt_sfx;
	Mix_Chunk * step_sfx[4];
	
}; 

#endif
