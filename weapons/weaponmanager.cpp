/*
 * weaponmanager.cpp
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


#include "weaponmanager.h"


WeaponManager::WeaponManager(SDL_Renderer** renderer,  ObjectManager * theStuff)
{
		context = renderer;
		objs = theStuff;
}

void WeaponManager::ReadWeapons(float shot_scale){

	char source_file[5][50];
	int shots;
	int speed;
	int faster;
	int damage;
	int distance;
	int sw,sh,dw,dh,swi,shi,dwi,dhi;
	
	Weapon * temp;
	
	FILE * fp;

	fp = fopen (WEAPON_SOURCES, "r");

	for(int i = 0; i < MAXWEAPONS; i++){
		fscanf(fp, "%s %s %s %s %s %d %d %d %d %d %d %d %d %d %d %d %d %d\n", &(source_file[0][0]),&(source_file[1][0]),
																		   &(source_file[2][0]),&(source_file[3][0]),
																		   &(source_file[4][0]),&shots,&distance,&speed,&faster,
																		   &damage,&sw,&sh,&dw,&dh,&swi,&shi,&dwi,&dhi);		

		source_weapons_textures[i][0] = IMG_LoadTexture(*context, &(source_file[0][0]));

		if (source_weapons_textures[i][0] == nullptr)
		{
			printf("IMG_LoadTexture Error: %s\n", IMG_GetError() );
			exit(1);
		}
		source_weapons_rect[i][0].x = 0; source_weapons_rect[i][0].y = 0;
		source_weapons_rect[i][0].w = sw; source_weapons_rect[i][0].h = sh;
		
		if(strcmp("null",&(source_file[1][0]))){

			source_weapons_textures[i][1] = IMG_LoadTexture(*context, &(source_file[1][0]));
			if (source_weapons_textures[i][1] == nullptr)
			{
				printf("IMG_LoadTexture Error: %s\n", IMG_GetError() );
				exit(1);
			}		
			
			source_weapons_textures[i][2] = IMG_LoadTexture(*context, &(source_file[2][0]));
			if (source_weapons_textures[i][2] == nullptr)
			{
				printf("IMG_LoadTexture Error: %s\n", IMG_GetError() );
				exit(1);
			}		
			
			source_weapons_rect[i][1].x = 0; source_weapons_rect[i][1].y = 0;
			source_weapons_rect[i][1].w = swi; source_weapons_rect[i][1].h = shi;
			initial_weapons_dst[i].w = dwi; initial_weapons_dst[i].h = dhi;

		}
		else{
			source_weapons_textures[i][1] = source_weapons_textures[i][2] = nullptr;
		}
		//Load sound effects 
		weapon_sounds[i] = Mix_LoadWAV(&(source_file[3][0])); 
		if( weapon_sounds[i] == NULL ) { 
			printf( "Failed to load weapon sound effect! SDL_mixer Error: %s\n", Mix_GetError() ); 
			exit(-1); 
		}
		weapon_hits[i] = Mix_LoadWAV(&(source_file[4][0])); 
		if( weapon_hits[i] == NULL ) { 
			printf( "Failed to load weapon sound effect! SDL_mixer Error: %s\n", Mix_GetError() ); 
			exit(-1); 
		}

		stats[i][0] = speed + (WALK_SPEED+RUN_SPEED)*faster;
		stats[i][1] = damage;
		stats[i][2] = dw;
		stats[i][3] = dh;
		stats[i][4] = distance*TILE_SIZE;

		for(int j = 0; j < (int)((float)shots*shot_scale); j++){
			temp = new Weapon; 
			temp->type = i;
			temp->active = 0;
			temp->dst.w = dw;
			temp->dst.h = dh;
			weaps[i].Insert((void *)temp,i);
		}
	}


}

void WeaponManager::UpdateWeapons(int wx0, int wy0){
	// Update velocities
	UpdateVelocities();
	// Check collisions
	ObjectCollisions(wx0, wy0);	
	// Draw shapes
	DrawWeapons(wx0,wy0);
}

Node * WeaponManager::AvailibleShot(int t){
	Node * temp = weaps[t].head;
		
	while(temp)
	{
		if(!(((Weapon *)(temp->obj))->active))return temp;
		temp = temp->next;
	}
	
	return temp;
}

void WeaponManager::DrawWeapons(int wx0, int wy0){
	Node * temp;

	temp = actives.head;
	while(temp)
	{
		((Weapon *)(((Node *)(temp->obj))->obj))->DrawShape(context, &source_weapons_rect[temp->codes[0]][0],
															&source_weapons_textures[temp->codes[0]][0], wx0,wy0);
		temp = temp->next;
	}
	
}

void WeaponManager::UpdateVelocities(){

	Node * temp = actives.head;
	while(temp)
	{
		((Weapon *)(((Node *)(temp->obj))->obj))->x0 += ((Weapon *)(((Node *)(temp->obj))->obj))->xdir ? -stats[temp->codes[0]][0] : stats[temp->codes[0]][0];
		((Weapon *)(((Node *)(temp->obj))->obj))->distance += stats[temp->codes[0]][0];

		if(((Weapon *)(((Node *)(temp->obj))->obj))->distance > stats[temp->codes[0]][4]){
				Node * tempy = temp->next;
				actives.Remove(temp);
				((Weapon *)(((Node *)(temp->obj))->obj))->active = 0;
				
				delete temp;
				temp = tempy;
		}
		else{
			temp = temp->next;
		}
	}
}


void WeaponManager::ObjectCollisions(int wx0, int wy0){

	int xcol, ycol;
	Node * temp_o;
	Node * tempy;
	int x, y, w, h;
	int cx,cy,cw,ch;
	Node * temp_w = actives.head;

	while(temp_w)
	{					
		temp_o = objs->collision_tiles.head;

		while(temp_o){
			x = ((Weapon *)(((Node *)(temp_w->obj))->obj))->x0;
			y = ((Weapon *)(((Node *)(temp_w->obj))->obj))->y0;
			w = stats[temp_w->codes[0]][2];
			h = stats[temp_w->codes[0]][3];
			if(((Tile *)(((Node *)(temp_o->obj))->obj))->NarrowCollision(x,y,w,h) ||
				!(wx0 < x + w && wx0 + WINDOW_X > x && wy0 < y + h && wy0 + WINDOW_Y > y)){
				tempy = temp_w->next;
				actives.Remove(temp_w);
				((Weapon *)(((Node *)(temp_w->obj))->obj))->active = 0;

				Mix_PlayChannel( -1, weapon_hits[temp_w->codes[0]], 0 );
				
				delete temp_w;
				temp_w = tempy;
				goto anyways1;
			}
			temp_o = temp_o->next;
		}
		temp_w = temp_w->next;
		
		anyways1:
		continue;
	}
}

void WeaponManager::EnemyCollision(int wx0, int wy0, Creature * thing,int r, int g, int b){
	int xcol, ycol;
	Node * tempy;
	int x, y, w, h;
	int cx,cy,cw,ch;
	Node * temp_w = actives.head;

	while(temp_w){

		x = ((Weapon *)(((Node *)(temp_w->obj))->obj))->x0;
		y = ((Weapon *)(((Node *)(temp_w->obj))->obj))->y0;
		w = stats[temp_w->codes[0]][2];
		h = stats[temp_w->codes[0]][3];
		
		cx = thing->locx;
		cy = thing->locy;
		ch = thing->h;
		cw = thing->w;


		if(  thing->health_cur > 0 
		  && ( cx < x + w )
		  && ( cx + cw > x )
		  && ( cy < y + h )
		  && ( cy + ch > y)){

			color.r = r; color.g = g; color.b = b;
			//sprintf(string, "%d",stats[temp_w->codes[0]][1]);
			//FixedPrint(*context,((Weapon *)(((Node *)(temp_w->obj))->obj))->x0, ((Weapon *)(((Node *)(temp_w->obj))->obj))->y0, string, &color, 2);
			//printf("----> %s",string);
			

			thing->health_cur -= stats[temp_w->codes[0]][1];
			tempy = temp_w->next;
			actives.Remove(temp_w);
			((Weapon *)(((Node *)(temp_w->obj))->obj))->active = 0;

			Mix_PlayChannel( -1, weapon_hits[temp_w->codes[0]], 0 );

			delete temp_w;
			temp_w = tempy;
			goto anyways2;
	
		}
		temp_w = temp_w->next;
		anyways2:
		continue;
	}
}

void WeaponManager::FireWeapons(int type, int x0, int y0, int xdir, int ydir, int wx0, int wy0){
	printf("%d\n",type);
	if (type == WEAPON_0){
		
		Node * temp = AvailibleShot(WEAPON_0);
		if(temp){
			((Weapon *)(temp->obj))->active = 1;
			((Weapon *)(temp->obj))->distance = 0;
			((Weapon *)(temp->obj))->x0 = x0;
			((Weapon *)(temp->obj))->y0 = y0;
			((Weapon *)(temp->obj))->xdir = xdir;
			((Weapon *)(temp->obj))->ydir = ydir;
			Mix_PlayChannel( -1, weapon_sounds[temp->codes[0]], 0 );
			if(source_weapons_textures[temp->codes[0]][1] != nullptr){
				// update initial location for more generic placement
				initial_weapons_dst[temp->codes[0]].x = x0 - wx0;
				initial_weapons_dst[temp->codes[0]].y = y0 - wy0 - 10;
				
				if(xdir)
					SDL_RenderCopy(*context, source_weapons_textures[temp->codes[0]][1], &source_weapons_rect[temp->codes[0]][1], &initial_weapons_dst[temp->codes[0]]);
				else
					SDL_RenderCopy(*context, source_weapons_textures[temp->codes[0]][2], &source_weapons_rect[temp->codes[0]][1], &initial_weapons_dst[temp->codes[0]]);
			}

			actives.Insert((void *)temp,0);

		}
	}

}

void WeaponManager::DeleteWeapons(){

	Node * temp;
	
	for(int i = 0; i < MAXWEAPONS; i++)
		while((temp = weaps[i].PopHead())){
			delete ((Weapon *)temp->obj);
			delete temp;
		}
		// free sound data TODO
}
