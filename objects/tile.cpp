#include "tile.h"

Tile::Tile(int nT, int xN0, int yN0, int dw, int dh,float scale){
	type = nT;
	dst.x = xN0; dst.y = yN0; dst.w = (int)((float)dw*scale); dst.h = (int)((float)dh*scale);

	xl = xN0; xr = xN0 + dw;
	yt = yN0; yb = yN0 + dh;
}

void Tile::DefineTile(int nT, int xN0, int yN0,
					  int dw, int dh){
	
	type = nT;
	dst.x = xN0; dst.y = yN0; dst.w = dw; dst.h = dh;
	xl = xN0; xr = xN0 + dw;
	yt = yN0; yb = yN0 + dh;
	
}

void Tile::DrawShape(SDL_Renderer** renderer, SDL_Rect * source, SDL_Texture ** texture, int wx0, int wy0){

	dst.x = xl - wx0;
	dst.y = yt - wy0;
								 
	SDL_RenderCopy(*renderer, *texture, source, &dst);


}

int Tile::OnScreen(int wx0, int wy0){

	return (wx0 < xr &&
			wx0 + WINDOW_X > xl &&
			wy0 < yb &&
			wy0 + WINDOW_Y > yt);
}

int Tile::OnCollsion(int wx0, int wy0){

	return (wx0 - WINDOW_X/2 < xr &&
			wx0 + WINDOW_X + WINDOW_X/2 > xl &&
			wy0 - WINDOW_Y/2 < yb &&
			wy0 + WINDOW_Y + WINDOW_Y/2 > yt);
}

int Tile::NarrowCollision(int px0, int py0, int pw0, int ph0){

	return ((px0) < xr &&
			(px0) + pw0 > xl &&
			(py0) < yb &&
			(py0) + ph0 > yt);

}

void Tile::MoveCenter(){

}

void Tile::PrintParameters(){
	printf("%d %d %d %d\n",xl,xr,yt,yb);
}
