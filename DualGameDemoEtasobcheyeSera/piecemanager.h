#ifndef PIECE_MANAGER_H
#define PIECE_MANAGER_H

#include "game.h"

#define PIECE_WIDTH 100
#define PIECE_HEIGHT 100

// Chess Tokens 
enum ChessToken {
	KING,
	QUEEN,
	BISHOP,
	KNIGHT,
	ROOK,
	PAWN,
	EMPTY
};

// Chess Piece
struct ChessPiece {
	ChessToken token;
	SDL_Rect location;
	std::pair<char, int> locationNot;
	bool playerID;
	std::set<std::pair<char, int>> validMoves;
};


class PieceManager
{
public:
	PieceManager(SDL_Renderer* renderer);
	~PieceManager();

	void render(SDL_Renderer* renderer);

	void movePiece(SDL_Point selectedPoint, SDL_Point mousePosition);
	void snapPiece(SDL_Point selectedPoint, SDL_Point mousePosition);

	bool checkValidPickup(SDL_Point selectedPoint);


private:
	void SpriteSheetInit(SDL_Renderer* renderer);
	void drawSprite(int spriteID, SDL_Renderer* renderer, SDL_Rect* dst);

	void initializeValidMoves();
	void recomputeValidMoves();
	bool kingSafe(std::pair<char, int> src, std::pair<char, int> dst);
	bool checkmateChecker();

	std::pair<char, int> getTileID(int x, int y);
	
	std::map<std::pair<char, int>, ChessPiece> pieces;

	SDL_Texture* spriteSheet;
	std::vector<SDL_Rect> sprites;

	bool turn;

};

#endif