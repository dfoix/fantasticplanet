#ifndef ENEMY_H
#define ENEMY_H

#define ENEMY_SOURCES "monsters/enemy_sources.txt"
#define MAXENEMIES 4
#define RESPAWN_DELAY 10

#define SLIME_CODE 0
#define SLIMEVEL_X 4
#include "slime.h"

#define FACE_CODE 1
#define FACEVEL_X 2
#define FACEVEL_Y 1
#define FACEDAMAGE 10
#include "face.h"

#define APE_CODE 2
#define APE_SPEED (WALK_SPEED+RUN_SPEED)/2
#define APE_SHOT_SCALE .5
#define APE_SHOT_DELAY 10
#define APE_DISTANCE 3*TILE_SIZE
#include "ape.h"

#define JUMPER_CODE 3
#define JUMPER_DELAY 7
#define JUMPER_DAMAGE 5
#include "jumper.h"
#endif
