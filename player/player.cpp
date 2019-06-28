#include "player.h"

//
// Author:		Daniel Foix
// Function:	Player()
// Accepts:		Nothing
// Returns:		Nothing
// Purpose:		Constructor to initialize all shape parameters
// Modified:	09/30/2018
//
Player::Player(SDL_Renderer** renderer, Background * nBG,
			   ObjectManager * nOBJS){
	context = renderer;	
	bg = nBG;
	objs = nOBJS;
	weaps = new WeaponManager(renderer,nOBJS);

	w = GD_W; h = GD_H;
	dst.x = 100; dst.y = 0;  dst.w = GD_W; dst.h = GD_H;
	src.x = 0; src.y = 51; src.w = 51; src.h = 51;
	wx0 = 0;
	wy0 = 0;
	locx = dst.x;
	locy = dst.y;
	fall_on = jump_on = 0;
	pose_num = 7;
	pose_cur = 0;
	pose_length = 3;
}

 void Player::Setup(){
	char str[32] = HEALTHBAR_FILE00;	
	weaps->ReadWeapons(1);
	for(int i = 0; i < 10;i++){
		str[24] = i + '0';
		h_textures[i] = IMG_LoadTexture(*context, str);
		if (h_textures[i] == nullptr)
		{
			printf("IMG_LoadTexture Error: %s\n", IMG_GetError() );
			exit(1);
		}
	}
	
	h_textures[10] = IMG_LoadTexture(*context, HEALTHBAR_FILE10);
	if (h_textures[10] == nullptr)
	{
		printf("IMG_LoadTexture Error: %s\n", IMG_GetError() );
		exit(1);
	}		

	jump_sfx = Mix_LoadWAV(JUMP_SFX); 
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
	
	h_dst.x = HEALTH_X; h_dst.y = HEALTH_Y;  h_dst.w = HEALTH_W; h_dst.h = HEALTH_H;
	h_src.x = 0; h_src.y = 0; h_src.w = HEALTH_FILE_W; h_src.h = HEALTH_FILE_H;
}

void Player::PlacePlayer(int px, int py){
	locx = px; 
	locy = py;
	
	//wx0 = px - (WINDOW_X/2 - GD_W/2);
	//wy0 = py - (WINDOW_Y/2 - GD_H/2);
	//	dst.x =  WINDOW_X/2 - GD_W/2; 
	//	dst.y =  WINDOW_Y/2 - GD_H/2; 


	if(locx  <= WINDOW_X/2 - GD_W/2){
		wx0 = 0;
		dst.x = locx; 
	}
	else if(locx + GD_W/2 + WINDOW_X/2  >= objs->level_x){
		wx0 = objs->level_x - WINDOW_X;
		dst.x = locx+WINDOW_X-objs->level_x; 	
	}
	else{
		wx0 = locx - (WINDOW_X/2 - GD_W/2);
		dst.x = WINDOW_X/2 - GD_W/2; 	
	}

	if(locy  <= WINDOW_Y/2 - GD_H/2){
		wy0 = 0;
		dst.y = locy; 
	}
	else if(locy + GD_H/2 + WINDOW_Y/2  >= objs->level_y){
		wy0 = objs->level_y - WINDOW_Y;
		dst.y = locy+WINDOW_Y-objs->level_y; 	
	}
	else{
		wy0 = locy - (WINDOW_Y/2 - GD_H/2);
		dst.y = WINDOW_Y/2 - GD_H/2; 	
	}

	bg->SetWX(wx0);
	bg->SetWY(wy0);




}

void Player::CalcVelocity() {
//	static float air_time = 0;
	const Uint8 *keyboard = SDL_GetKeyboardState(NULL);
	
	switch(fall_on || jump_on)
	{
	
	case true:
		if(active){
			if (keyboard[SDL_SCANCODE_D]){
				xdir = 0;x_vel = WALK_SPEED + RUN_SPEED*keyboard[SDL_SCANCODE_LSHIFT];}
			if (keyboard[SDL_SCANCODE_A]){
				xdir = 1;x_vel = -WALK_SPEED- RUN_SPEED*keyboard[SDL_SCANCODE_LSHIFT];}
			if(!keyboard[SDL_SCANCODE_D] && !keyboard[SDL_SCANCODE_A])
				x_vel = 0;
			if(!keyboard[SDL_SCANCODE_W] && jump_on && air_time > SDL_DELAY && y_vel < 0){
				jump_on = 0;
				fall_on = 1;
				air_time = 0;
			}
			if(air_time > SDL_DELAY ) state = STATE_JUMP;
		}	else{
			x_vel = xdir ?  -DAMAGE_SPEED_X : DAMAGE_SPEED_X;
		}		air_time += SDL_DELAY;
		
		y_vel = UpdateYVel((float)air_time,active ? (jump_on ? JUMP_SPEED : 0) : DAMAGE_SPEED_Y);
		ydir = y_vel >= 0;
		break;

	case false:
		if(active){
			if(!keyboard[SDL_SCANCODE_W])
			{
				if (keyboard[SDL_SCANCODE_D]) {
					x_vel = WALK_SPEED + RUN_SPEED*keyboard[SDL_SCANCODE_LSHIFT];
					xdir = 0;
					state = STATE_RUN;
				}
				if (keyboard[SDL_SCANCODE_A]) {
					x_vel = -WALK_SPEED - RUN_SPEED*keyboard[SDL_SCANCODE_LSHIFT];
						xdir = 1;
					state = STATE_RUN;
				}

				if (keyboard[SDL_SCANCODE_S]){
					x_vel = 0;
					state = STATE_CROUCH;
				}
			
				if(!keyboard[SDL_SCANCODE_D] && !keyboard[SDL_SCANCODE_A] && 
				   !keyboard[SDL_SCANCODE_S]){
					x_vel = 0;
					state = STATE_STAND;
				}
				fall_on = 1;
				y_vel = UpdateYVel(SDL_DELAY,JUMP_SPEED);

			}
			else
			{
				air_time = 0;
				jump_on = 1;
				if(x_vel)state = STATE_RUN;
				else if(!x_vel)state = STATE_STAND;
			}
		}
		else{
			y_vel = UpdateYVel(SDL_DELAY,DAMAGE_SPEED_Y);
		}
		break;
	}
}

void Player::UpdatePosition(int * adv) {

	int xcol = 0,ycol = 0;

///////////////////////////////////////
// collision check here
	objs->Collisions(wx0,wy0,locx + C_X_OFFSET,locy + C_Y_OFFSET,C_W,C_H,&x_vel,&y_vel,&xcol,&ycol);	

	if(x_vel){
		locx += x_vel;
		if(locx  <= WINDOW_X/2 - GD_W/2 && wx0 + x_vel > 0 && dst.x >= WINDOW_X/2 - GD_H/2) {
			wx0 += x_vel;
			bg->SetWX(wx0);printf("check %d\n",wx0);
		}
		else if(locx  <= WINDOW_X/2 - GD_W/2){
			dst.x += x_vel;
		}

		else if(locx + GD_W/2 + WINDOW_X/2  >= objs->level_x && wx0 + x_vel  < objs->level_x - WINDOW_X && dst.x <= WINDOW_X/2 - GD_H/2) {
			wx0 += x_vel;
			bg->SetWX(wx0);
		}
		else if(locx + GD_W/2 + WINDOW_X/2  >= objs->level_x){
			dst.x += x_vel;		
		}

		else if(wx0 + x_vel  >= objs->level_x - WINDOW_X) {
			wx0 += x_vel - (WINDOW_X/2 - GD_W/2 - dst.x);
			bg->SetWX(wx0);
			dst.x = WINDOW_X/2 - GD_H/2;
		}

		else if(wx0 + x_vel <= 0) {
			wx0 += x_vel -(WINDOW_X/2 - GD_W/2 - dst.x);
			bg->SetWX(wx0);
			dst.x = WINDOW_X/2 - GD_W/2;
		}
		else {
			wx0 += x_vel;
			bg->SetWX(wx0);
		}
	}
	if(xcol){
		x_vel = 0;
	}

	if(y_vel){
		locy += y_vel;
		if(locy  <= WINDOW_Y/2 - GD_H/2 && wy0 + y_vel > 0 && dst.y >= WINDOW_Y/2 - GD_H/2) {
			wy0 += y_vel;
			
			bg->SetWY(wy0);
		}
		else if(locy  <= WINDOW_Y/2 - GD_H/2){
			dst.y += y_vel;
		}
		else if(locy + GD_H/2 + WINDOW_Y/2  >= objs->level_y && wy0 + y_vel < objs->level_y - WINDOW_Y && dst.y <= WINDOW_Y/2 - GD_H/2) {
			wy0 += y_vel;
			bg->SetWY(wy0);
		}
		else if(locy + GD_H/2 + WINDOW_Y/2  >= objs->level_y){
			dst.y += y_vel;	
		}
		else if(wy0 +y_vel <= 0) {
			wy0 += y_vel -(WINDOW_Y/2 - GD_H/2 - dst.y);
			bg->SetWY(wy0);
			dst.y = WINDOW_Y/2 - GD_H/2;

		}

		else if(locy + GD_H/2 + WINDOW_Y/2 < objs->level_y && locy + GD_H/2 + WINDOW_Y/2 >= objs->level_y - y_vel ) {
			wy0 = locy - WINDOW_Y/2 + GD_H/2;
			bg->SetWY(wy0);
			dst.y = WINDOW_Y/2 - GD_H/2;
		}
		else {
			wy0 += y_vel;
			bg->SetWY(wy0);
		}
	}
	if(ycol) {
		y_vel = 0;
		jump_on = 0;
		if(air_time > 0)
			Mix_PlayChannel( -1, land_sfx, 0 );

		fall_on = ydir <=0;
		air_time = 0;
		if(!active)xdir = 1-xdir;
		active = 1;
		//state = STATE_STAND;	
	}
	else if(air_time == 0 && jump_on == 1)
		Mix_PlayChannel( -1, jump_sfx, 0 );
	
	if (!((locx) < objs->level_x &&
		  (locx) + GD_W > 0 &&
		  (locy) < objs->level_y &&
		  (locy) + GD_H > 0)){
		if(locx > objs->level_x/4)
			*adv = 1;
		else
			*adv = -1;
		return;
	}
	else{
		*adv = 0;
	}

	DrawPlayer();


#ifdef C_TEST
	// draws four lines to create a square centered at x0/y0
	int R = 255, G = 0, B = 0;
	SDL_SetRenderDrawColor(*context, R, G, B, SDL_ALPHA_OPAQUE);

	SDL_RenderDrawLine(*context, dst.x+C_X_OFFSET,       dst.y+C_Y_OFFSET,
								 dst.x+C_X_OFFSET+C_W, dst.y+C_Y_OFFSET);
	SDL_RenderDrawLine(*context, dst.x+C_X_OFFSET+C_W, dst.y+C_Y_OFFSET,
							     dst.x+C_X_OFFSET+C_W, dst.y+C_Y_OFFSET+C_H);
	SDL_RenderDrawLine(*context, dst.x+C_X_OFFSET+C_W, dst.y+C_Y_OFFSET+C_H ,
								 dst.x+C_X_OFFSET      , dst.y+C_Y_OFFSET+C_H);
	SDL_RenderDrawLine(*context, dst.x+C_X_OFFSET      , dst.y+C_Y_OFFSET+C_H,
								 dst.x+C_X_OFFSET      , dst.y+C_Y_OFFSET);

#endif	

}

void Player::DrawPlayer(){
	bg->RefreshScreen();
	objs->DrawBackgroundTiles(wx0,wy0);
	SDL_RenderCopy(*context, texture, &src, &dst);
}

void Player::UpdateFrame(){
	pose_timer++;		
	if (pose_timer >= pose_length)
	{
		pose_cur = pose_cur >= pose_num ? 0 : pose_cur + 1;
		pose_timer = 0;
	}
	ChangeFrames();
}

void Player::ChangeFrames(){
	static int i = 0, j = 0;
	switch (state)
	{
		case STATE_CROUCH:
		src.y = 0;
		src.x = xdir*51 + 102;
		pose_num = 0;		
		break;
		
		case STATE_HURT:
		src.y = 0;
		src.x = (1-xdir)*51 + 306;
		pose_num = 0;	
		break;
		
		case STATE_JUMP:
		src.y = 0;
		src.x = 4*51 + xdir*51;
		pose_num = pose_length = 0;
		break;
		
		case STATE_RUN:
		src.y = 51 + 51*xdir; 
		src.x = pose_cur * 51;
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
		src.y = 0;
		src.x = xdir*51;
		pose_num = 1;
		break;
	}
}

void Player::ManageWeapons(){
	if(!active)return;
	const Uint8 *keyboard = SDL_GetKeyboardState(NULL);
	if (keyboard[WEAPON_0_KEY] && !keystat[WEAPON_0]){
		weaps->FireWeapons(WEAPON_0,locx + (1-xdir)*GUN_X,locy + GUN_Y + 15*(state == STATE_CROUCH) ,xdir,ydir,wx0,wy0);
		keystat[WEAPON_0] = 1;
	}
	else if (!keyboard[WEAPON_0_KEY] && keystat[WEAPON_0]){
		keystat[WEAPON_0] = 0;
	}
}

int Player::IsActive(){
	return active;
}

void Player::Hurt(int x0, int y0, int damage){

	health_cur -= damage;
	active = 0;		
	if(locx < x0)
		xdir = 1;
	else
		xdir = 0;
	state = STATE_HURT;
	jump_on = 1;
	
	Mix_PlayChannel( -1, hurt_sfx, 0 );

}

void Player::GameOver(){
	bg->RefreshScreen();
	objs->DrawBackgroundTiles(wx0,wy0);
	SDL_RenderCopy(*context, texture, &src, &dst);
}

