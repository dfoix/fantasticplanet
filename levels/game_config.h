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

#ifndef LEVEL1_CONFIG
#define LEVEL1_CONFIG

#define TITLE "Fantastic Planet"

#define WINDOW_X 640
#define WINDOW_Y 400

#define BACK_W 240
#define BACK_H 176
#define MID_W 272
#define MID_H 176
#define TILE_SIZE 64

#define DEAD_MUSIC "levels/music/dead.mp3"
#define VICTORY_MUSIC "levels/music/victory.mp3"
#define THEME_MUSIC "levels/music/maintheme.mp3"

#define SCROLL_SFX "mainmenu/sounds/scroll.wav"
#define SELECT_SFX "mainmenu/sounds/select.wav"

#define INTRO_GRAPHIC "mainmenu/graphics/FP.png"
#define PLANET_W 256
#define PLANET_H 256

#endif
