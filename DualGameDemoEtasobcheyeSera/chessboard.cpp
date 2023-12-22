#include "chessboard.h"

ChessBoard::ChessBoard()
{
	chessTiles = new Tile * [8];
	for (int i = 0; i < 8; i++)
		chessTiles[i] = new Tile[8];

	// Tile Colors
	SDL_Color c1 = { 238, 216, 192, 255 };
	SDL_Color c2 = { 171, 122, 101, 255 };

	// Initialize Tiles
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			// Set ID
			chessTiles[i][j].tileID = j + (i * 8);

			// Set Location
			chessTiles[i][j].location = { j * TILE_WIDTH, i * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT };

			// Set Color
			if (i % 2 == 0)
				if (j % 2 == 0)
					chessTiles[i][j].color = c1;
				else
					chessTiles[i][j].color = c2;
			else
				if (j % 2 == 0)
					chessTiles[i][j].color = c2;
				else
					chessTiles[i][j].color = c1;

		}
	}

	printf("Chess Board Initialized...\n");

}

ChessBoard::~ChessBoard()
{
	// Free Up Memory
	for (int i = 0; i < 8; i++)
	{
		delete(chessTiles[i]);
	}
	delete(chessTiles);
}

void ChessBoard::render(SDL_Renderer* renderer)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			// Set Tile Color
			SDL_Color color = chessTiles[i][j].color;
			SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

			// Draw Tile
			SDL_RenderFillRect(renderer, &chessTiles[i][j].location);
		}
	}
}
