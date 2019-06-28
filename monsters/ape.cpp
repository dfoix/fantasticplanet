#include "ape.h"

//
// Author:		Daniel Foix
// Function:	Ape()
// Accepts:		Nothing
// Returns:		Nothing
// Purpose:		Constructor to initialize all shape parameters
// Modified:	09/30/2018
//
Ape::Ape(SDL_Renderer** renderer, ObjectManager * stuff, int x, int y, int nw, int nh, int health){
	context = renderer;
	objs = stuff;
	weaps = new WeaponManager(renderer,stuff);

	locx = x;
	locy = y;
	w = nw;
	h = nh;
	dst.w = w;
	dst.h = h;
	health_cur = health_max = health;
	fall_on = jump_on = 0;
}

 void Ape::Setup(Player * guy){
	dude = guy;
	jump_sfx = Mix_LoadWAV(JUMP_SFX); 
	weaps->ReadWeapons(APE_SHOT_SCALE);
	if( jump_sfx == NULL ) { 
		printf( "Failed to load jump sound effect! SDL_mixer Error: %s\n", Mix_GetError() ); 
		exit(-1); 
	}
	land_sfx = Mix_LoadWAV(LAND_SFX); 
	if( land_sfx == NULL ) { 
		printf( "Failed to load land sound effect! SDL_mixer Error: %s\n", Mix_GetError() ); 
		exit(-1); 
	}
	hurt_sfx = Mix_LoadWAV(HURT_SFX); 
	if( hurt_sfx == NULL ) { 
		printf( "Failed to load hurt sound effect! SDL_mixer Error: %s\n", Mix_GetError() ); 
		exit(-1); 
	}
	
	for(int i = 0; i<STEP_NUM;i++){
		char str[50];
		sprintf(str,"%s%d.wav",STEP_SFX,i);
		step_sfx[i] = Mix_LoadWAV(str); 
		if( step_sfx[i] == NULL ) { 
			printf( "Failed to load step sound effect! SDL_mixer Error: %s\n", Mix_GetError() ); 
			exit(-1); 
		}	
	}
}

int Ape::IsActive(int wx0, int wy0, int gx, int gy) {

		// returns if face is onscreen. update later for more complex activity
	return (wx0 - WINDOW_X/4 < locx + w  &&
			wx0 + WINDOW_X + WINDOW_X/4 > locx  &&
			wy0 - WINDOW_Y/4 < locy + h &&
			wy0 + WINDOW_Y + WINDOW_Y/4 > locy);
}

void Ape::CalcVelocity() {
//	static float air_time = 0;
	const Uint8 *keyboard = SDL_GetKeyboardState(NULL);
	
	switch(fall_on || jump_on)
	{
	
	case true:
			if (dude->locx>locx){
				xdir = 0;x_vel = APE_SPEED;}
			if (dude->locx<locx){
				xdir = 1;x_vel = -APE_SPEED;}
			if(!keyboard[SDL_SCANCODE_D] && !keyboard[SDL_SCANCODE_A]){
				x_vel = 0;
			
				if (dude->locx>locx)
					xdir = 0;
				else
					xdir = 1;
				
			}
			if( jump_on && air_time > SDL_DELAY && y_vel < 0){
				jump_on = 0;
				fall_on = 1;
				air_time = 0;
			}
			if(air_time > SDL_DELAY ) state = STATE_JUMP;
				air_time += SDL_DELAY;
		
		y_vel = UpdateYVel((float)air_time,jump_on ? JUMP_SPEED : 0);
		ydir = y_vel >= 0;
		break;

	case false:
		if (dude->locx>locx) {
			x_vel = APE_SPEED;
			xdir = 0;
			state = STATE_RUN;
		}
		if (dude->locx<locx) {
			x_vel = -APE_SPEED;
				xdir = 1;
			state = STATE_RUN;
		}
	
		if( dude->locx > locx - APE_DISTANCE && dude->locx < locx + APE_DISTANCE){
			x_vel = 0;
			if (dude->locx>locx)
				xdir = 0;
			else
				xdir = 1;
			
			state = STATE_STAND;
		}
		fall_on = 1;
		y_vel = UpdateYVel(SDL_DELAY,JUMP_SPEED);

		break;
	}
}

void Ape::UpdatePosition(int wx0, int wy0) {


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
		if(air_time > 0)
			Mix_PlayChannel( -1, land_sfx, 0 );
		fall_on = ydir <=0;
		air_time = 0;
		//state = STATE_STAND;	
	}
	else if(air_time == 0 && jump_on == 1)
		Mix_PlayChannel( -1, jump_sfx, 0 );	
	dst.x = locx - wx0;
	dst.y = locy - wy0;

//	dst.x = 300;
	//dst.y = 220;
}

void Ape::UpdateFrame(SDL_Rect * src){
	pose_timer++;		
	if (pose_timer >= pose_length)
	{
		pose_cur = pose_cur >= pose_num ? 0 : pose_cur + 1;
		pose_timer = 0;
	}
	ChangeFrames(src);
}

void Ape::ChangeFrames(SDL_Rect * src){
	static int i = 0, j = 0;
	switch (state)
	{
		case STATE_CROUCH:
		src->y = 0;
		src->x = xdir*51 + 102;
		pose_num = 0;		
		break;
		
		case STATE_HURT:
		src->y = 0;
		src->x = (1-xdir)*51 + 306;
		pose_num = 0;	
		break;
		
		case STATE_JUMP:
		src->y = 0;
		src->x = 4*51 + xdir*51;
		pose_num = pose_length = 0;
		break;
		
		case STATE_RUN:
		src->y = 51 + 51*xdir; 
		src->x = pose_cur * 51;
		pose_num = 7;
		pose_length = 3;
		
		// TODO: should be placed somewhere else.
		// this function should only decide frames, not sfx 
		if(air_time == 0){
			if(j++ > STEP_FRAME_DELAY){
				j = 0;
				Mix_PlayChannel( -1, step_sfx[i], 0 );
				i = (i+1)%STEP_NUM;
			}
		}
		
		break;
				
		case STATE_STAND:
		src->y = 0;
		src->x = xdir*51;
		pose_num = 1;
		break;
	}
}

void Ape::ManageWeapons(int wx0, int wy0){
	static int i = 0;
	if(i++ >= APE_SHOT_DELAY && dude->locy + GD_H/2 >= locy){
		i = 0;
		weaps->FireWeapons(WEAPON_0,locx + (1-xdir)*GUN_X,locy + GUN_Y,xdir,ydir,wx0,wy0);
	}
	weaps->EnemyCollision(wx0,wy0,dude,255,0,0);

}
