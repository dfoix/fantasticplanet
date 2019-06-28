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
#include "../levels/game_config.h"

#ifndef PLAYER_CONFIG
#define PLAYER_CONFIG

#define WALK_SPEED 6
#define RUN_SPEED 2

#define TURN_SPEED 3
#define SDL_DELAY 50
//#define GRAV_ACCEL 65
//#define JUMP_SPEED 50
#define GRAV_ACCEL 85
#define JUMP_SPEED 60
#define DAMAGE_SPEED_X 6
#define DAMAGE_SPEED_Y 40
#define TERMINAL_VEL 30

#define GD_W 120
#define GD_H 120

#define C_W 50
#define C_H 100
#define C_CROUCH 10

#define C_X_OFFSET (GD_W - C_W)/2
#define C_Y_OFFSET (GD_H - C_H)

#define HEALTHBAR_FILE00 "player/graphics/health_000.png"
#define HEALTHBAR_FILE01 "player/graphics/health_010.png"
#define HEALTHBAR_FILE02 "player/graphics/health_020.png"
#define HEALTHBAR_FILE03 "player/graphics/health_030.png"
#define HEALTHBAR_FILE04 "player/graphics/health_040.png"
#define HEALTHBAR_FILE05 "player/graphics/health_050.png"
#define HEALTHBAR_FILE06 "player/graphics/health_060.png"
#define HEALTHBAR_FILE07 "player/graphics/health_070.png"
#define HEALTHBAR_FILE08 "player/graphics/health_080.png"
#define HEALTHBAR_FILE09 "player/graphics/health_090.png"
#define HEALTHBAR_FILE10 "player/graphics/health_100.png"

#define JUMP_SFX "player/sounds/jump.wav"
#define LAND_SFX "player/sounds/land.wav"
#define HURT_SFX "player/sounds/hurt_swosh.wav"
#define STEP_SFX "player/sounds/step_"

#define STEP_NUM 4
#define STEP_FRAME_DELAY 7

#define HEALTH_FILE_W 292
#define HEALTH_FILE_H 32
#define HEALTH_W WINDOW_X/4
#define HEALTH_H WINDOW_Y/20

#define HEALTH_X 20
#define HEALTH_Y 20

//#define C_TEST

#endif
