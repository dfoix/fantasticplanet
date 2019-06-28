#include <time.h>
#include <SDL2/SDL.h>
#include "Fixed_print.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "levels/game_config.h"
#include "player/player_config.h"
#include "objects/object.h"
#include "objects/tile.h"
#include "objects/objectmanager.h"
#include "creatures/creature.h"
#include "player/player.h"
#include "backgrounds/background.h"
#include "weapons/weapon.h"
#include "weapons/weaponmanager.h"
#include "monsters/slime.h"
#include "monsters/enemymanager.h"

#define ever (;;)

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event event;
Mix_Music *bg_music = nullptr;
Background screen(&renderer);
ObjectManager objs(&renderer); 
Player gd(&renderer,&screen,&objs);
EnemyManager foes(&renderer, &gd, &objs,gd.weaps);

void MainMenu();
void AdvanceLevel(int level,int adv);
void setupSDL();
void postSDLCleanup();

int main(int argc, char* argv[])
{
	int won = 0;
	SDL_Rect blank;
	blank.x = 0; blank.y = 0;
	blank.w = WINDOW_X; blank.h = WINDOW_Y;
	
	char string[100];
	SDL_Color color;
	
	int i = 0, px,py, adv = 1, level = 0,gameon,op=0;
	time_t score = time(nullptr);
 	setupSDL();

	gd.Setup();
	gd.SetTexture("player/graphics/gooddude2 (copy).png");
	objs.LoadTiles();			
	foes.ReadEnemies();

	MainMenu();
	long time;
	long offset = SDL_GetTicks();
	for ever
	{ 

		if(adv){
			level += adv;
			AdvanceLevel(level,adv);
			adv = 0;
		}
		
		if(!won && op <255 && op >= 0 && gd.UpdatePlayer(&adv)){
			won = foes.UpdateEnemies(gd.wx0,gd.wy0,gd.locx,gd.locy);
			objs.DrawMidgroundTiles(gd.wx0,gd.wy0);
			objs.DrawForegroundTiles(gd.wx0,gd.wy0);
			gd.ManageWeapons();
			gd.UpdateHealth();
		}
		else if (op <255 && op >= 0){
			op+=7;
			gd.DrawPlayer();
			gd.weaps->DrawWeapons(gd.wx0,gd.wy0);
			foes.DrawEnemies(gd.wx0,gd.wy0,gd.locx,gd.locy);	
			objs.DrawMidgroundTiles(gd.wx0,gd.wy0);
			objs.DrawForegroundTiles(gd.wx0,gd.wy0);
			SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, op > 255 ? 255:op);
			SDL_RenderFillRect(renderer,&blank);
			SDL_RenderDrawRect(renderer,&blank);
		}
		else if(op >= 255){
			time = (SDL_GetTicks() - offset)/1000;
			SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_NONE);
			SDL_RenderDrawRect(renderer,&blank);
			if( Mix_PlayingMusic() != 0 ) { 
				Mix_PauseMusic();
				if(bg_music != nullptr)
					Mix_FreeMusic( bg_music );		 
			}
			
			if(won)
				bg_music = Mix_LoadMUS(VICTORY_MUSIC); 
			else
				bg_music = Mix_LoadMUS(DEAD_MUSIC); 
			
			if( bg_music == NULL ) { 
				printf( "Failed to load background music! SDL_mixer Error: %s\n", Mix_GetError() ); 
				exit(-1);
			}

			if( Mix_PlayingMusic() == 0 ) { 
				if( Mix_PlayMusic(bg_music, -1) == -1 ) { 
					exit(-1); 
				} 
			} 
			
			op = -1;
			color.r = 255; color.g = 255; color.b = 255;
		}
		else if(op < 0){
			
	
			if(won){
				sprintf(string, "Mission accomplished.");
				FixedPrint(renderer,230, 150, string, &color, 2);
				sprintf(string, "Mission time: %d seconds.",time);
				FixedPrint(renderer,130, 180, string, &color, 2);
			}
			else{
				sprintf(string, "You are dead.");
				FixedPrint(renderer,230, 150, string, &color, 2);
				sprintf(string, "It took you %d seconds to die.",time);
				FixedPrint(renderer,130, 180, string, &color, 2);
			}
		}
		SDL_RenderPresent(renderer);	
		
		if (SDL_PollEvent(&event) > 0)
		{
		const Uint8 *state = SDL_GetKeyboardState(NULL);
			if (state[SDL_SCANCODE_ESCAPE]) goto finale;
			switch(event.type)
			{
			case SDL_QUIT: 
				goto finale;
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_Delay(SDL_DELAY);
		
		i++;
	}

finale:
	postSDLCleanup();
	objs.DeleteObjects();
	foes.DeleteEnemies();
	foes.DeleteEnemies();
	gd.weaps->DeleteWeapons();
}

void MainMenu(){
	
	char string[100];
	SDL_Color color;
	
	int i=0, j=0,f=0,state = 0,op=255,key=0,delay = 0;

	SDL_Texture* planet;
	SDL_Rect src, dst;
/*
	Mix_Chunk * scroll;
	scroll = Mix_LoadWAV("mainmenu/sounds/scroll.wav"); 
	if( scroll == NULL ) { 
		printf( "Failed to load scroll sound effect! SDL_mixer Error: %s\n", Mix_GetError() ); 
		exit(-1); 
	}

	Mix_Chunk * select;
	select = Mix_LoadWAV("mainmenu/sounds/select.wav"); 
	if( select == NULL ) { 
		printf( "Failed to load select sound effect! SDL_mixer Error: %s\n", Mix_GetError() ); 
		exit(-1); 
	}
*/
	SDL_Rect blank;
	blank.x = 0; blank.y = 0;
	blank.w = WINDOW_X; blank.h = WINDOW_Y;
	
	if( Mix_PlayingMusic() != 0 ) { 
		Mix_PauseMusic();
		if(bg_music != nullptr)
			Mix_FreeMusic( bg_music );		 
	}
			
	bg_music = Mix_LoadMUS(THEME_MUSIC); 
	
	if( bg_music == NULL ) { 
		printf( "Failed to load background music! SDL_mixer Error: %s\n", Mix_GetError() ); 
		exit(-1);
	}

	if( Mix_PlayingMusic() == 0 ) { 
		if( Mix_PlayMusic(bg_music, -1) == -1 ) { 
			exit(-1); 
		} 
	} 

	planet = IMG_LoadTexture(renderer, INTRO_GRAPHIC);

	if (!planet)
	{
		printf("IMG_LoadTexture Error: %s\n", IMG_GetError() );
		exit(1);
	}
	
	dst.x = WINDOW_X/2 - PLANET_W; dst.y = WINDOW_Y/2 - PLANET_H; 
	dst.w = PLANET_W*2; dst.h = PLANET_H*2;
	src.w = PLANET_W; src.h = PLANET_H;


	while(true){		
		src.x = (i%5)*256;
		src.y = j*256;
		SDL_RenderCopy(renderer, planet, &src, &dst);
		if(op>0 && !state){
			op -= 5;
			SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, op);
			SDL_RenderFillRect(renderer,&blank);
			SDL_RenderDrawRect(renderer,&blank);

		}
		

		switch(state){
			case(1):	
			color.r = 255; color.g = 255*(1-(key==0)); color.b = 255*(1-(key==0));
			sprintf(string, "BEGIN");
			FixedPrint(renderer,50, 100, string, &color, 2);
			
			color.r = 255; color.g = 255*(1-(key==1)); color.b = 255*(1-(key==1));
			sprintf(string, "PROLOGUE");
			FixedPrint(renderer,100, 150, string, &color, 2);
			
			color.r = 255; color.g = 255*(1-(key==2)); color.b = 255*(1-(key==2));
			sprintf(string, "CONTROLS");
			FixedPrint(renderer,150, 200, string, &color, 2);
			
			color.r = 255; color.g = 255*(1-(key==3)); color.b = 255*(1-(key==3));
			sprintf(string, "CREDITS");
			FixedPrint(renderer,200, 250, string, &color, 2);





			break;
			
			case 2:
			color.r = 255; color.g = 255; color.b = 255;
			blank.w = blank.h = dst.w;
			blank.x = dst.x;
			blank.y = dst.y;
			SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
			SDL_RenderFillRect(renderer,&blank);
			SDL_RenderDrawRect(renderer,&blank);
			
			sprintf(string, "Date: 2089/04/13");
			FixedPrint(renderer,50, 50, string, &color, 2);			
			sprintf(string, "From the desk of Col. Straker:");
			FixedPrint(renderer,50, 70, string, &color, 2);
			sprintf(string, "An experimental autonomous planetary exploration");
			FixedPrint(renderer,50, 120, string, &color, 2);			
			sprintf(string, "(APE) chassis malefunctioned during a routine cave");
			FixedPrint(renderer,50, 140, string, &color, 2);		
			sprintf(string, "analysis.");
			FixedPrint(renderer,50, 160, string, &color, 2);		

			sprintf(string, "You are required to disable the APE before its");
			FixedPrint(renderer,50, 200, string, &color, 2);		
			sprintf(string, "uranium core reaches critical mass, causing");
			FixedPrint(renderer,50, 220, string, &color, 2);		
			sprintf(string, "irreparable damage to the alien ruins.");
			FixedPrint(renderer,50, 240, string, &color, 2);		

			sprintf(string, "Time is of the essence.");
			FixedPrint(renderer,50, 280, string, &color, 2);		
			break;
			
			case 3:
			color.r = 255; color.g = 0; color.b = 0;
			sprintf(string, "WARNING: Modifications to APE suit control systems ");
			FixedPrint(renderer,50, 40, string, &color, 2);
			sprintf(string, "         are prohibitited as of 2079/01/23");
			FixedPrint(renderer,50, 60, string, &color, 2);
			color.g = 255; color.b = 255;
			sprintf(string, "W -> Jump");
			FixedPrint(renderer,50, 110, string, &color, 2);
			sprintf(string, "D -> Move right");
			FixedPrint(renderer,50, 140, string, &color, 2);			
			sprintf(string, "A -> Move left");
			FixedPrint(renderer,50, 170, string, &color, 2);			
			sprintf(string, "S -> Crouch");
			FixedPrint(renderer,50, 200, string, &color, 2);			
			sprintf(string, "Space -> Attack");
			FixedPrint(renderer,50, 230, string, &color, 2);			
			sprintf(string, "LShift -> Run");
			FixedPrint(renderer,50, 260, string, &color, 2);
			break;

			case 4:
			color.r = 255; color.g = 255; color.b = 255;
			sprintf(string, "Programming: Daniel Foix");
			FixedPrint(renderer,50, 110, string, &color, 2);

			sprintf(string, "TFT Library: Dr. Evan Schemm");
			FixedPrint(renderer,50, 140, string, &color, 2);


			sprintf(string, "All art, music, and sound effects were ");
			FixedPrint(renderer,50, 200, string, &color, 2);
			sprintf(string, "provided by public domain sources.");
			FixedPrint(renderer,50, 230, string, &color, 2);
			color.g = 0; color.b = 0;
			sprintf(string, "Please view the readme.txt for proper attribution.");
			FixedPrint(renderer,50, 280, string, &color, 2);

			
			break;
			
		}
		SDL_RenderPresent(renderer);	
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_Delay(SDL_DELAY);
		
		if(!(f=(f+1)%3)){
			i = (i+1)%25;
			j = i%5 == 0 ? (j+1)%5 : j;
		}		

		if (SDL_PollEvent(&event) > 0)
		{
			switch(event.type)
			{
			case SDL_KEYDOWN: 
				if(!state){
					state = 1;
					dst.w = dst.h = WINDOW_Y/2;
					dst.x = WINDOW_X - WINDOW_Y/2;
					dst.y = WINDOW_Y/4;
				}
				break;
			case SDL_QUIT:
				postSDLCleanup();
				objs.DeleteObjects();
				foes.DeleteEnemies();
				foes.DeleteEnemies();
				gd.weaps->DeleteWeapons();
				exit(0);
				break;
			}
		}
		

			if(delay++ > 2){
				delay = 0;
				const Uint8 *keyboard = SDL_GetKeyboardState(NULL);
				if(state > 1){
					if (keyboard[SDL_SCANCODE_RETURN] || 
						keyboard[SDL_SCANCODE_SPACE] ||
						keyboard[SDL_SCANCODE_ESCAPE] ||
						keyboard[SDL_SCANCODE_BACKSPACE]){
						state = 1;
						dst.w = dst.h = WINDOW_Y/2;
						dst.x = WINDOW_X - WINDOW_Y/2;
						dst.y = WINDOW_Y/4;

					}
				}
				if(state == 1){
					if (keyboard[SDL_SCANCODE_UP] || keyboard[SDL_SCANCODE_W])
						key = key - 1 < 0 ? 3 : key -1;
					if (keyboard[SDL_SCANCODE_DOWN] || keyboard[SDL_SCANCODE_S])
						key = (key+1)%4;
					if (keyboard[SDL_SCANCODE_RETURN] || keyboard[SDL_SCANCODE_SPACE]){
						if(key==0)return;
						state = key + 1;
					}

			}
		}
	}
	
}
//				Mix_FreeChunk(scroll);
//				Mix_FreeChunk(select);
	


void AdvanceLevel(int level,int adv){
	
	static char str[6][50] = {"\0","\0","\0","\0"};
	int px,py,bg; 

	sprintf(&(str[0][0]),"levels/l%02d/l%02d_config.txt",level,level);
	sprintf(&(str[1][0]),"levels/l%02d/l%02d_background.csv",level,level);
	sprintf(&(str[2][0]),"levels/l%02d/l%02d_midground.csv",level,level);
	sprintf(&(str[3][0]),"levels/l%02d/l%02d_foreground.csv",level,level);

	bg = objs.ReadLevelTiles(&px,&py,adv,&(str[4][0]),&(str[0][0]),&(str[1][0]),&(str[2][0]),&(str[3][0]));

	if(strcmp(&(str[5][0]),&(str[4][0]))){

		if( Mix_PlayingMusic() != 0 ) { 
			Mix_PauseMusic();
			if(bg_music != nullptr)
				Mix_FreeMusic( bg_music );		 
		}
		
		bg_music = Mix_LoadMUS(&(str[4][0])); 
		
		if( bg_music == NULL ) { 
			printf( "Failed to load background music! SDL_mixer Error: %s\n", Mix_GetError() ); 
			exit(-1);
		}

		if( Mix_PlayingMusic() == 0 ) { 
			if( Mix_PlayMusic(bg_music, -1) == -1 ) { 
				exit(-1); 
			} 
		} 
		
		strcpy(&(str[5][0]),&(str[4][0]));
	}
	
	printf("%d\n",bg);
	if(bg > 0){
		sprintf(&(str[0][0]),"backgrounds/graphics/background%02d.png",bg);
		sprintf(&(str[1][0]),"backgrounds/graphics/midground%02d.png",bg);
	
		screen.SetTextures(&(str[0][0]),&(str[1][0]),BACK_W,BACK_H,MID_W,MID_H);								
		screen.active = 1;
	}
	else
		screen.active = 0;

	sprintf(&(str[0][0]),"monsters/levels/l%02d.txt",level);

	foes.UpdateEnemyList(&(str[0][0]));

	gd.PlacePlayer(px,py);

}


void setupSDL()
{
	if (SDL_Init(SDL_INIT_TIMER|SDL_INIT_AUDIO|SDL_INIT_VIDEO) != 0)
	{
		printf("SDL failed to init: %s\n", SDL_GetError());
		exit(1);
	}

	// Create window and renderer
	window = SDL_CreateWindow(TITLE, 
			    SDL_WINDOWPOS_UNDEFINED, 
			    SDL_WINDOWPOS_UNDEFINED, 
			    WINDOW_X, WINDOW_Y, SDL_WINDOW_SHOWN);

	renderer = SDL_CreateRenderer(window, -1, 
				SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);

	if ((window == NULL) || (renderer == NULL))
	{
		printf("SDL_CreateWindowAndRenderer Error: %s\n", SDL_GetError() );
		exit(1);

	}
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) { 
		printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() ); 
		exit(1);
	}
	
	FixedPrint_Setup(); // initializes the FixedPrint library

}

void postSDLCleanup()                                        
{
	SDL_DestroyRenderer(renderer);                           
	SDL_DestroyWindow(window);                               
	Mix_Quit(); 
	IMG_Quit();
	SDL_Quit();                                              
}                                                        
