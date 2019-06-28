/*
 * objectmanager.cpp
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


#include "objectmanager.h"

ObjectManager::ObjectManager(SDL_Renderer** renderer){

	context = renderer;

}


void ObjectManager::LoadTiles(){
	Tile * temp;
	int xN0, yN0, w, h,s,scode,ccode, t,i=0,j=0;
	char source_file[30];
	
	FILE * fp;

	fp = fopen (TILE_SOURCES, "r");
printf("%d\n",MAXTILES);
	for(int i = 0; i < MAXTILES; i++){
		fscanf(fp, "%s %d %d %d\n", source_file,&w,&h,&ccode);
		source_tiles_textures[i] = IMG_LoadTexture(*context, source_file);

		if (source_tiles_textures[i] == nullptr)
		{
			printf("IMG_LoadTexture Error: %s\n", IMG_GetError() );
			exit(1);
		}
		source_tiles_rect[i].x = 0; source_tiles_rect[i].y = 0;
		source_tiles_rect[i].w = w; source_tiles_rect[i].h = h;
		
		collision_codes[i] = ccode;
	}

	fclose(fp);
}

int ObjectManager::ReadLevelTiles(int * px, int * py, int adv, char * sound, const char * level_config, const char * background_file, const char * midground_file, const char * foreground_file){

	Tile * temp;
	int xN0, yN0, w, h,scode, t,i=0,j=0,bg, tx1,ty1,tx2,ty2;
	FILE * fp;

	DeleteObjects();
	
	fp = fopen (level_config, "r");
	fscanf(fp, "%d %d %d %d %d %d %d\n",&bg,&w,&h,&tx1,&ty1,&tx2,&ty2);	
	fscanf(fp, "%s\n",sound);		
	fclose(fp);
	
	w *= TILE_SIZE;
	h *= TILE_SIZE;
	if(adv > 0){
		*px = tx1*TILE_SIZE;
		*py = ty1*TILE_SIZE;
	}
	else{
		*px = tx2*TILE_SIZE;
		*py = ty2*TILE_SIZE;
	}
	level_x = w; level_y = h;
	fp = fopen (background_file, "r");
	while(!feof(fp)){
			fscanf(fp,"%d",&scode);
			if(scode >= 0){
				temp = new Tile(scode, TILE_SIZE*i*OBJECT_SCALE, TILE_SIZE*j*OBJECT_SCALE - (source_tiles_rect[scode].h - TILE_SIZE), 
								source_tiles_rect[scode].w, source_tiles_rect[scode].h,OBJECT_SCALE);		
				background_tiles.Insert((void *)temp);
			}
		i++;
		if(getc(fp) == '\n'){i=0;j++;printf("\n");};
	}
	fclose(fp);
	i=j=0;
	fp = fopen (midground_file, "r");
	while(true){
			fscanf(fp,"%d",&scode);
			if(feof(fp))break;
			if(scode >= 0){
				temp = new Tile(scode, TILE_SIZE*i*OBJECT_SCALE, TILE_SIZE*j*OBJECT_SCALE - (source_tiles_rect[scode].h - TILE_SIZE), 
								source_tiles_rect[scode].w, source_tiles_rect[scode].h,OBJECT_SCALE);		
				tangible_tiles.Insert((void *)temp);
			}
		i++;
		if(getc(fp) == '\n'){i=0;j++;};
	}
	fclose(fp);
	i=j=0;
	fp = fopen (foreground_file, "r");
	while(!feof(fp)){
			fscanf(fp,"%d",&scode);
			if(scode >= 0){
				temp = new Tile(scode, TILE_SIZE*i*OBJECT_SCALE, TILE_SIZE*j*OBJECT_SCALE - (source_tiles_rect[scode].h - TILE_SIZE), 
								source_tiles_rect[scode].w, source_tiles_rect[scode].h,OBJECT_SCALE);		
				foreground_tiles.Insert((void *)temp);
			}
		i++;
		if(getc(fp) == '\n'){i=0;j++;};
	}
	fclose(fp);
	return bg;
}


void ObjectManager::DrawBackgroundTiles(int wx0,int wy0){
	static int test= 0;
	Node * temp = background_tiles.head;
	int tile_code;int i = 0;	
	while(temp){
		if(((Tile *)(temp->obj))->OnScreen(wx0,wy0)){
			tile_code = ((Tile *)(temp->obj))->type;
			((Tile *)(temp->obj))->DrawShape(context,&source_tiles_rect[tile_code],
													 &source_tiles_textures[tile_code],
													 wx0,wy0);

		}
//		printf("%d %d %d\n",((Tile *)(temp->obj))->xl,((Tile *)(temp->obj))->yt,tile_code);
//		std::cin.ignore();
		temp = temp->next;
	}

}

void ObjectManager::DrawMidgroundTiles(int wx0,int wy0){
	static int test= 0;
	Node * temp;
	int tile_code;

	
	while((temp = collision_tiles.PopHead()))
		delete temp;
	temp = tangible_tiles.head;
	while(temp){
		if(((Tile *)(temp->obj))->OnCollsion(wx0,wy0))	collision_tiles.Insert((void *)temp);
		if(((Tile *)(temp->obj))->OnScreen(wx0,wy0)){	
			tile_code = ((Tile *)(temp->obj))->type;
			((Tile *)(temp->obj))->DrawShape(context,&source_tiles_rect[tile_code],
													 &source_tiles_textures[tile_code],
													 wx0,wy0);
		}
		temp = temp->next;
	}
}

void ObjectManager::DrawForegroundTiles(int wx0,int wy0){
	static int test= 0;
	Node * temp = foreground_tiles.head;
	int tile_code;

	while(temp){
		if(((Tile *)(temp->obj))->OnScreen(wx0,wy0)){
			tile_code = ((Tile *)(temp->obj))->type;
			((Tile *)(temp->obj))->DrawShape(context,&source_tiles_rect[tile_code],
													 &source_tiles_textures[tile_code],
													 wx0,wy0);
		}
		temp = temp->next;
	}
}

void ObjectManager::Collisions(int wx0, int wy0, int px0, int py0, int pw0, int ph0, float * xvel, float * yvel, int * xcol, int * ycol){
	CollisionTiles(wx0, wy0, px0, py0, pw0, ph0, xvel, yvel, xcol, ycol);
}

void ObjectManager::CollisionTiles(int wx0, int wy0, int px0, int py0, int pw0, int ph0, float * xvel, float * yvel, int * xcol, int * ycol){
	Node * temp;
	*xcol = *ycol = 0;

	temp = collision_tiles.head;

	while(temp){
//		((Tile *)((Node *)(temp->obj))->obj)->NarrowCollision(px0,py0,pw0,ph0, xvel,yvel,xcol, ycol);
//exit(-1);
		collision_types[collision_codes[((Tile *)((Node *)(temp->obj))->obj)->type]](((Tile *)((Node *)(temp->obj))->obj)->xl,
																					 ((Tile *)((Node *)(temp->obj))->obj)->xr,
																					 ((Tile *)((Node *)(temp->obj))->obj)->yt,
																					 ((Tile *)((Node *)(temp->obj))->obj)->yb,
																					 px0,py0,pw0,ph0, xvel,yvel,xcol, ycol);

		if(*xcol && *ycol)
			return;		
		temp = temp->next;
	}

	return;
}

void ObjectManager::PrintTiles(){
	Node * temp = background_tiles.head;
	printf("\n\nbackground tiles:\n"
		   "==================");
	while(temp){
		printf("%d %d %d %d %d\n",((Tile *)(temp->obj))->type,
								((Tile *)(temp->obj))->xl,
								((Tile *)(temp->obj))->xr,
								((Tile *)(temp->obj))->yt,
								((Tile *)(temp->obj))->yb);
		temp = temp->next;
	}
	temp = tangible_tiles.head;
	printf("tangible tiles:\n"
		   "==================\n");
	while(temp){
		printf("%d %d %d %d %d\n",((Tile *)(temp->obj))->type,
								((Tile *)(temp->obj))->xl,
								((Tile *)(temp->obj))->xr,
								((Tile *)(temp->obj))->yt,
								((Tile *)(temp->obj))->yb);
		temp = temp->next;
	}

	temp = foreground_tiles.head;
	printf("foreground tiles:\n"
		   "==================\n");
	while(temp){
		printf("%d %d %d %d %d\n",((Tile *)(temp->obj))->type,
								((Tile *)(temp->obj))->xl,
								((Tile *)(temp->obj))->xr,
								((Tile *)(temp->obj))->yt,
								((Tile *)(temp->obj))->yb);
		temp = temp->next;

	}
}

void ObjectManager::DeleteObjects(){
	Node * temp;
	
	while((temp = tangible_tiles.PopHead())){
		delete ((Tile *)temp->obj);
		delete temp;
	}

	while((temp = background_tiles.PopHead())){
		delete ((Tile *)temp->obj);
		delete temp;
	}

	while((temp = foreground_tiles.PopHead())){
		delete ((Tile *)temp->obj);
		delete temp;
	}

	while((temp = collision_tiles.PopHead())){
		delete temp;
	}
}
