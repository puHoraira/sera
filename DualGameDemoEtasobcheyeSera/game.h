#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <vector>
#include <set>
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Forward Declare of Chess Board Class
class ChessBoard;

// Forward Declare of Piece Manager Class
class PieceManager;

class Game
{
public:
	Game();
	~Game();

	void start();
	void handleEvents();
	void update();
	void render();

private:

	void initializeSDL();

	bool isRunning;

	// Time Attributes
	int FPS;
	int frameDelay;

	// Mouse
	SDL_Point mousePosition;
	SDL_Point selectedPosition;
	bool buttonDown;
	bool buttonUpTrigger;

	// Move Rules
	bool validMove;

	SDL_Window* window;
	SDL_Renderer* renderer;

	ChessBoard* board;
	PieceManager* pieceManager;
};

#endif