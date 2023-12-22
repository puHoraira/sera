#include "game.h"
#include "chessboard.h"
#include "piecemanager.h"

Game::Game()
{
	// Initialize Time
	FPS = 60;
	frameDelay = 1000 / FPS;

	// Initialize Rules
	validMove = false;

	// Initialize Mouse
	mousePosition = { 0,0 };
	selectedPosition = { 0,0 };
	buttonDown = false;
	buttonUpTrigger = false;

	// Initialize SDL
	initializeSDL();

	// Initialize Chess Board
	board = new ChessBoard();

	// Initialize Piece Manager
	pieceManager = new PieceManager(renderer);

	// Game isn't running yet
	isRunning = false;
}

Game::~Game()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	printf("SDL Cleaned Up...\n");
}

void Game::start()
{
	isRunning = true;

	// Time Vars
	Uint32 frameStart;
	int frameTime;

	while (isRunning)
	{
		// Get Frame Start
		frameStart = SDL_GetTicks();

		handleEvents();
		update();
		render();

		// Handle Framerate
		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime)
			SDL_Delay(frameDelay - frameTime);
		
	}
}

void Game::handleEvents()
{
	SDL_Event event;

	SDL_PollEvent(&event);
	switch (event.type)
	{
		case SDL_QUIT:
			isRunning = false;
			break;

		case SDL_MOUSEMOTION:
			mousePosition = { event.motion.x, event.motion.y };
			break;

		case SDL_MOUSEBUTTONUP:
			if (buttonDown && event.button.button == SDL_BUTTON_LEFT)
			{
				buttonUpTrigger = true;
				buttonDown = false;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			if (!buttonDown && event.button.button == SDL_BUTTON_LEFT)
			{
				buttonDown = true;
				selectedPosition = mousePosition;
				validMove = pieceManager->checkValidPickup(selectedPosition);
			}
			break;

	}
}

void Game::update()
{
	if (validMove && buttonDown)
	{
		pieceManager->movePiece(selectedPosition, mousePosition);
	}

	if (buttonUpTrigger && validMove)
	{
		pieceManager->snapPiece(selectedPosition, mousePosition);
		buttonUpTrigger = false;
	}


}

void Game::render()
{
	// Clear Screen
	SDL_RenderClear(renderer);

	// Draw Chess Board
	board->render(renderer);

	// Draw Chess Pieces
	pieceManager->render(renderer);

	// Update Screen
	SDL_RenderPresent(renderer);
}

void Game::initializeSDL()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("SDL Initialization Failed!\n");
		exit(0);
	}
	printf("SDL Initialization Complete...\n");

	window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, 0);
	if (window)
		printf("Window Created!...\n");
	else {
		printf("Window Creation Failed!\n");
		exit(0);
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer)
		printf("Renderer Created!...\n");
	else {
		printf("Renderer Creation Failed\n");
		exit(0);
	}
}
