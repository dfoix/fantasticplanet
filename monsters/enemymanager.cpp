/*
 * enemy_manager.cpp
 * 
 * Copyright 2018 d <deimos@mercury>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include "enemymanager.h"

#define DEBUG_ENEMIES

EnemyManager::EnemyManager(SDL_Renderer** renderer, Player * theDude, ObjectManager * theStuff, WeaponManager * boom)
{
	dude = theDude;
	context = renderer;
	objs = theStuff;
	weaps = boom;
}

void EnemyManager::ReadEnemies(){
	char source_file[30];
	int sw,sh,dw,dh,health;
	
	FILE * fp;
	
	fp = fopen (ENEMY_SOURCES, "r");

	for(int i = 0; i < MAXENEMIES; i++){
		fscanf(fp, "%s %d %d %d %d %d\n", source_file,&sw,&sh, &dw, &dh, &health);		
		source_enemy_textures[i] = IMG_LoadTexture(*context, source_file);

		if (source_enemy_textures[i] == nullptr)
		{
			printf("IMG_LoadTexture Error: %s\n", IMG_GetError() );
			exit(1);
		}
		source_enemy_rect[i].x = 0; source_enemy_rect[i].y = 0;
		source_enemy_rect[i].w = sw; source_enemy_rect[i].h = sh;

		stats[i][0] = dw; stats[i][1] = dh; stats[i][2] = health;
	}
}

void EnemyManager::UpdateEnemyList(const char * level_file){
	char source_file[30];
	int t,x,y,n,a, spawn;
	Node * temp;
	FILE * fp;

	DeleteEnemies();

	fp = fopen (level_file, "r");
	fscanf(fp, "%d\n", &a);		
		
	for(int i = 0; i < a; i++){
		fscanf(fp, "%d %d %d %d\n", &t,&x,&y,&spawn);
		x *= TILE_SIZE;
		y *= TILE_SIZE;
		switch(t){
			case(SLIME_CODE):
				level_enemies.Insert((void *)(new Slime(context,objs,x,y,stats[t][0],stats[t][1],stats[t][2])),t,x,y,spawn);
				break;
			case(FACE_CODE):
				level_enemies.Insert((void *)(new Face(context,objs,x,y,stats[t][0],stats[t][1],stats[t][2])),t,x,y,spawn);
				break;				
			case(APE_CODE):
				temp = level_enemies.Insert((void *)(new Ape(context,objs,x,y,stats[t][0],stats[t][1],stats[t][2])),t,x,y,spawn);
				((Ape *)(temp->obj))->Setup(dude);
				break;				
			case(JUMPER_CODE):
				level_enemies.Insert((void *)(new Jumper(context,objs,x,y,stats[t][0],stats[t][1],stats[t][2])),t,x,y,spawn);
				break;				
#ifdef DEBUG_ENEMIES
			default:
				printf("not all enemy cases were covered");
				exit(-1);
#endif	
		}
	}
}

int EnemyManager::UpdateEnemies(int wx0, int wy0, int gx, int gy){
	Node * temp = level_enemies.head;
	int won = 0;
	while(temp){
		
//TODO implement update array
//		update_ptr func = ((Monster *)(temp->obj))->update_types[temp->codes[0]];
//		(((Monster *)(temp->obj))->*func)(wx0,  wy0,  gx, gy);
		
		weaps->EnemyCollision(wx0,wy0,((Creature *)temp->obj),0,0,0);
		switch(temp->codes[0]){
			case(SLIME_CODE):
				if(((Slime *)(temp->obj))->IsActive( wx0,  wy0,  gx, gy)){
				if(((Slime *)temp->obj)->health_cur <= 0){
					if(temp->codes[3]){
						if(temp->codes[3] > 0){
							temp->codes[3] = -1;
							temp->t = time(nullptr) + RESPAWN_DELAY;
						}
						else {
							if(time(nullptr) > temp->t) {
								temp->codes[3] = 1;
								((Slime *)temp->obj)->health_cur = ((Slime *)temp->obj)->health_max;
								((Slime *)temp->obj)->locx = temp->codes[1];
								((Slime *)temp->obj)->locy = temp->codes[2];
							}
						}
					}
					else{
						Node * temp2 = temp;
						temp = temp->next;
						delete ((Slime *)(temp2->obj));
						delete level_enemies.Remove(temp2);
						goto anyways;
					}
				}
				else{						
					((Slime *)temp->obj)->UpdatePosition(wx0,wy0,gx,gy,GD_W,GD_H,&source_enemy_rect[temp->codes[0]]);
					SDL_RenderCopy(*context, source_enemy_textures[temp->codes[0]], &source_enemy_rect[temp->codes[0]], &(((Slime *)temp->obj)->dst));
				}
			}
			break;

			case(FACE_CODE):
				if(((Face *)(temp->obj))->IsActive( wx0,  wy0,  gx, gy)){
					if(((Face *)temp->obj)->health_cur <= 0){
						if(temp->codes[3]){
							if(temp->codes[3] > 0){
								temp->codes[3] = -1;
								temp->t = time(nullptr) + RESPAWN_DELAY;
							}
							else {
								if(time(nullptr) > temp->t) {
									temp->codes[3] = 1;
									((Face *)temp->obj)->health_cur = ((Face *)temp->obj)->health_max;
									((Face *)temp->obj)->locx = temp->codes[1];
									((Face *)temp->obj)->locy = temp->codes[2];
								}
							}
						}
						else{
							Node * temp2 = temp;
							temp = temp->next;
							delete ((Face *)(temp2->obj));
							delete level_enemies.Remove(temp2);
							goto anyways;
						}
					}
					else{						
						if(((Face *)temp->obj)->UpdatePosition(wx0,wy0,gx,gy,GD_W,GD_H,&source_enemy_rect[temp->codes[0]])){
							if(!((Face *)temp->obj)->intersection){
								dude->Hurt(((Face *)temp->obj)->locx,((Face *)temp->obj)->locy,FACEDAMAGE);
								((Face *)temp->obj)->intersection = 1;
							}
						}
						else ((Face *)temp->obj)->intersection = 0;
						SDL_RenderCopy(*context, source_enemy_textures[temp->codes[0]], &source_enemy_rect[temp->codes[0]], &(((Face *)temp->obj)->dst));

					}
				}
				break;
			case(APE_CODE):
				if(((Ape *)(temp->obj))->health_cur > 0){
					((Ape *)(temp->obj))->UpdateApe(wx0, wy0, &source_enemy_rect[temp->codes[0]]);
				}
				else{
					won = 1;
				}
				SDL_RenderCopy(*context, source_enemy_textures[temp->codes[0]], &source_enemy_rect[temp->codes[0]], &(((Ape *)temp->obj)->dst));
				break;


			case(JUMPER_CODE):
				if(((Jumper *)(temp->obj))->IsActive( wx0,  wy0,  gx, gy)){
					if(((Jumper *)temp->obj)->health_cur <= 0){
						if(temp->codes[3]){
							if(temp->codes[3] > 0){
								temp->codes[3] = -1;
								temp->t = time(nullptr) + RESPAWN_DELAY;
							}
							else {
								if(time(nullptr) > temp->t) {
									temp->codes[3] = 1;
									((Jumper *)temp->obj)->health_cur = ((Jumper *)temp->obj)->health_max;
									((Jumper *)temp->obj)->locx = temp->codes[1];
									((Jumper *)temp->obj)->locy = temp->codes[2];
								}
							}
						}
						else{
							Node * temp2 = temp;
							temp = temp->next;
							delete ((Jumper *)(temp2->obj));
							delete level_enemies.Remove(temp2);
							goto anyways;
						}
					}
					else{						
						if(((Jumper *)temp->obj)->UpdatePosition(wx0,wy0,gx,gy,GD_W,GD_H,&source_enemy_rect[temp->codes[0]])){
							if(!((Jumper *)temp->obj)->intersection){
								dude->Hurt(((Jumper *)temp->obj)->locx,((Jumper *)temp->obj)->locy,JUMPER_DAMAGE);
								((Jumper *)temp->obj)->intersection = 1;
							}
						}
						else ((Jumper *)temp->obj)->intersection = 0;
						SDL_RenderCopy(*context, source_enemy_textures[temp->codes[0]], &source_enemy_rect[temp->codes[0]], &(((Jumper *)temp->obj)->dst));

					}
				}
				break;


				
#ifdef DEBUG_ENEMIES
			default:
				printf("not all enemy cases were covered");
				exit(-1);
#endif	
		}
		temp = temp->next;
		anyways:
		continue;
	}
	
	return won;
}

void EnemyManager::DeleteEnemies(){
	Node * temp;

	while((temp = level_enemies.PopHead())){
		switch(temp->codes[0]){
			case 0:
			delete ((Slime *)temp->obj);
			break;

			case 1:
			delete ((Face *)temp->obj);
			break;
			
			case 2:
			((Ape *)temp->obj)->weaps->DeleteWeapons();
			delete ((Ape *)temp->obj);
			break;
			
		}
		delete temp;
	}
}

void EnemyManager::DrawEnemies(int wx0, int wy0, int gx, int gy){
	Node * temp;

	temp = level_enemies.head;
	while(temp)
	{
		switch(temp->codes[0]) {
			case 0:
			if(((Slime *)(temp->obj))->IsActive( wx0,  wy0,  gx, gy)){
				SDL_RenderCopy(*context, source_enemy_textures[temp->codes[0]], &source_enemy_rect[temp->codes[0]], &(((Slime *)temp->obj)->dst));
			}
			break;
			
			case 1:
			if(((Face *)(temp->obj))->IsActive( wx0,  wy0,  gx, gy)){
				SDL_RenderCopy(*context, source_enemy_textures[temp->codes[0]], &source_enemy_rect[temp->codes[0]], &(((Face *)temp->obj)->dst));
			}
			break;
			case 2:
			if(((Ape *)(temp->obj))->IsActive( wx0,  wy0,  gx, gy)){
				SDL_RenderCopy(*context, source_enemy_textures[temp->codes[0]], &source_enemy_rect[temp->codes[0]], &(((Ape *)temp->obj)->dst));
			}
			break;
		}
	
		temp = temp->next;
	}
	
}
