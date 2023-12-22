#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include "game.h"

#define TILE_WIDTH 100
#define TILE_HEIGHT 100

struct Tile
{
	SDL_Color color;
	int tileID;
	SDL_Rect location;
};

class ChessBoard
{
public:
	ChessBoard();
	~ChessBoard();

	void render(SDL_Renderer* renderer);

private:
	Tile** chessTiles;

};

#endif