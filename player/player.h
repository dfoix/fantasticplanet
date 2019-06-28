////////////////////////////////////////////////////////////////////////
/////
/////	Author:		Daniel Foix
/////	Class:		Player
/////	Purpose:	Draw/track a circle onscreen
/////	Modified	09/30/2018
/////
////////////////////////////////////////////////////////////////////////

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "player_config.h"
#include "../backgrounds/background.h"
#include "../objects/objectmanager.h"
#include "../weapons/weaponmanager.h"
#include "../creatures/creature.h"
#include "../levels/game_config.h"

//#define DEBUG

#ifndef PLAYER_H
#define PLAYER_H

#define STATE_STAND 0
#define STATE_RUN 1
#define STATE_CROUCH 2
#define STATE_JUMP 3
#define STATE_HURT 4
//#define C_TEST
//#define DEBUG

using namespace std; 

class Player : public Creature
{
    // Access specifier 
    public: 
	Player(SDL_Renderer** renderer, Background * nBG,
		   ObjectManager * nOBJS);
	
	inline int UpdatePlayer(int * adv){
		if(health_cur <= 0) {GameOver(); return 0;}
		CalcVelocity();
		UpdateFrame();
		UpdatePosition(adv);
		weaps->UpdateWeapons(wx0,wy0);
		return 1;
	}
	//			((Weapon *)(temp->obj))->x0 = locx + (1-xdir)*GUN_X;
//			((Weapon *)(temp->obj))->y0 = locy + GUN_Y;
	void Setup();
	void PlacePlayer(int px, int py);
	void CalcVelocity();
	inline void UpdateHealth(){
		if(health_cur <= 0)
			SDL_RenderCopy(*context, h_textures[0], &h_src, &h_dst);
		else if(health_cur >= health_max)
			SDL_RenderCopy(*context, h_textures[10], &h_src, &h_dst);
		else{
			float percent= (float)health_cur/(float)health_max;
			SDL_RenderCopy(*context, h_textures[1 + (int)(percent*10)], &h_src, &h_dst);
		}
	}
	void UpdatePosition(int * adv);
	void UpdateFrame();
	void ChangeFrames();
	void ManageWeapons();
	void DrawPlayer();
	int IsActive();
	void Hurt(int x0, int y0, int damage);
	void GameOver();
	int wx0 = 0;
	int wy0 = 0;

	Background * bg;
	WeaponManager * weaps;
	private:
	int keystat[MAXWEAPONS] = {};
	SDL_Rect src;
	SDL_Texture * h_textures[11];
	SDL_Rect h_src,h_dst;
	Mix_Chunk * land_sfx;
	Mix_Chunk * jump_sfx;
	Mix_Chunk * hurt_sfx;
	Mix_Chunk * step_sfx[4];
	int active = 1;
	
}; 

#endif
