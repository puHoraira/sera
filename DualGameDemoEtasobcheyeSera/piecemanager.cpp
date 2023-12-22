#include "piecemanager.h"
#include "chess.h"

PieceManager::PieceManager(SDL_Renderer* renderer)
{
	// Chess Pieces for Either End
	ChessToken endFile[8] = { ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK };

	// White goes first
	turn = true;

	// Top Black Row
	for (int i = 0; i < 8; i++)
	{
		ChessPiece p;
		p.playerID = false; 
		p.token = endFile[i];

		SDL_Rect loc = { i * PIECE_WIDTH, 0, PIECE_WIDTH, PIECE_HEIGHT };
		p.location = loc;

		p.locationNot = getTileID(i, 0);

		pieces[p.locationNot] = p;
	}

	// Black Pawn Row
	for (int i = 0; i < 8; i++)
	{
		ChessPiece p;
		p.playerID = false;
		p.token = ChessToken::PAWN;

		SDL_Rect loc = { i * PIECE_WIDTH, PIECE_WIDTH, PIECE_WIDTH, PIECE_HEIGHT };
		p.location = loc;

		p.locationNot = getTileID(i, 1);

		pieces[p.locationNot] = p;
	}

	// Top White Row
	for (int i = 0; i < 8; i++)
	{
		ChessPiece p;
		p.playerID = true;
		p.token = endFile[i];

		SDL_Rect loc = { i * PIECE_WIDTH, 700, PIECE_WIDTH, PIECE_HEIGHT };
		p.location = loc;

		p.locationNot = getTileID(i, 7);

		pieces[p.locationNot] = p;
	}

	// White Pawn Row
	for (int i = 0; i < 8; i++)
	{
		ChessPiece p;
		p.playerID = true;
		p.token = ChessToken::PAWN;

		SDL_Rect loc = { i * PIECE_WIDTH, 700 - PIECE_WIDTH, PIECE_WIDTH, PIECE_HEIGHT };
		p.location = loc;

		p.locationNot = getTileID(i, 6);

		pieces[p.locationNot] = p;
	}


	// Initialize Sprite Sheet
	SpriteSheetInit(renderer);

	// Initialize Beginning Valid Moves
	initializeValidMoves();

}

PieceManager::~PieceManager()
{
}

void PieceManager::render(SDL_Renderer* renderer)
{
	std::map<std::pair<char, int>, ChessPiece>::iterator iter;
	for (iter = pieces.begin(); iter != pieces.end(); iter++)
	{
		ChessPiece p = iter->second;

		if (p.token != ChessToken::EMPTY)
		{
			switch (p.token)
			{
			case ChessToken::KING:
				if (p.playerID)
					drawSprite(0, renderer, &p.location);
				else
					drawSprite(6, renderer, &p.location);
				break;

			case ChessToken::QUEEN:
				if (p.playerID)
					drawSprite(1, renderer, &p.location);
				else
					drawSprite(7, renderer, &p.location);
				break;

			case ChessToken::BISHOP:
				if (p.playerID)
					drawSprite(2, renderer, &p.location);
				else
					drawSprite(8, renderer, &p.location);
				break;

			case ChessToken::KNIGHT:
				if (p.playerID)
					drawSprite(3, renderer, &p.location);
				else
					drawSprite(9, renderer, &p.location);
				break;

			case ChessToken::ROOK:
				if (p.playerID)
					drawSprite(4, renderer, &p.location);
				else
					drawSprite(10, renderer, &p.location);
				break;

			case ChessToken::PAWN:
				if (p.playerID)
					drawSprite(5, renderer, &p.location);
				else
					drawSprite(11, renderer, &p.location);
				break;
			}
		}



	}
}

void PieceManager::movePiece(SDL_Point selectedPoint, SDL_Point mousePosition)
{
	std::pair<char, int> loc = getTileID(selectedPoint.x / 100, selectedPoint.y / 100);
	
	pieces[loc].location.x = mousePosition.x - 50;
	pieces[loc].location.y = mousePosition.y - 50;
}

void PieceManager::snapPiece(SDL_Point selectedPoint, SDL_Point mousePosition)
{
	std::pair<char, int> loc = getTileID(selectedPoint.x / 100, selectedPoint.y / 100);
	std::pair<char, int> dst = getTileID(mousePosition.x / 100, mousePosition.y / 100);


	// Invalid Move
	if (pieces[loc].validMoves.find(dst) == pieces[loc].validMoves.end())
	{
		pieces[loc].location.x = (loc.first - 'a') * 100;
		pieces[loc].location.y = (8 - loc.second) * 100;
		return;
	}

	if (loc == dst) // Same Location
	{
		pieces[loc].location.x = (loc.first - 'a') * 100;
		pieces[loc].location.y = (8 - loc.second) * 100;
		return;
	}

	// Valid Move
	pieces.erase(dst);
	pieces[dst] = pieces[loc];
	pieces.erase(loc);
	pieces[dst].locationNot = dst;

	// Snap Location:
	pieces[dst].location.x = (dst.first - 'a') * 100;
	pieces[dst].location.y = (8 - dst.second) * 100;

	// Turn End
	turn = !turn;

	// Recompute Valid Moves
	recomputeValidMoves();

	// Checkmate?
	if (checkmateChecker())
	{
		printf("Checkmate!\n");
		SDL_Quit();
		startChess();
	}	
}

bool PieceManager::checkValidPickup(SDL_Point selectedPoint)
{
	// Get Selected Locale
	std::pair<char, int> loc = getTileID(selectedPoint.x / 100, selectedPoint.y / 100);

	if (pieces.find(loc) == pieces.end())
		return false;
	else
	{
		// Check Turn
		if (pieces[loc].playerID == turn)
			return true;
		else
			return false;
	}

}

void PieceManager::SpriteSheetInit(SDL_Renderer* renderer)
{
	// Create Surface from File
	SDL_Surface* tmpSurface = IMG_Load("ChessSprites.png");
	if (tmpSurface == NULL)
	{
		printf("Unable to Load Image %s!  SDL Image Error: %s\n", "ChessSprites.png", IMG_GetError());
		exit(1);
	}

	// Create Texture from Surface
	this->spriteSheet = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	if (this->spriteSheet == NULL)
	{
		printf("Unable to Create Texture %s!  SDL Error: %s\n", "ChessSprites.png", SDL_GetError());
		exit(1);
	}

	// Remove temporary surface
	SDL_FreeSurface(tmpSurface);

	// Get sheet dimensions
	int width, height;
	SDL_QueryTexture(this->spriteSheet, NULL, NULL, &width, &height);

	int rows = 2;
	int cols = 6;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			SDL_Rect tmpRect;
			tmpRect.h = height / rows;
			tmpRect.w = width / cols;

			tmpRect.x = j * tmpRect.w;
			tmpRect.y = i * tmpRect.h;

			this->sprites.push_back(tmpRect);
		}
	}
}

void PieceManager::drawSprite(int spriteID, SDL_Renderer* renderer, SDL_Rect* dst)
{
	SDL_RenderCopy(renderer, this->spriteSheet, &this->sprites[spriteID], dst);
}

void PieceManager::initializeValidMoves()
{
	std::map<std::pair<char, int>, ChessPiece>::iterator iter;
	for (iter = pieces.begin(); iter != pieces.end(); iter++)
	{
		std::pair<char, int> loc = iter->second.locationNot;
		char l = loc.first;

		switch (iter->second.token)
		{
			case ChessToken::PAWN:
				if (iter->second.playerID)
				{
					iter->second.validMoves.insert({l, 3});
					iter->second.validMoves.insert({l, 4});
				}
				else
				{
					iter->second.validMoves.insert({ l, 6 });
					iter->second.validMoves.insert({ l, 5 });
				}					
				break;

			case ChessToken::KNIGHT:
				if (iter->second.playerID)
				{
					iter->second.validMoves.insert({ l+1, 3 });
					iter->second.validMoves.insert({ l-1, 3 });
				}
				else
				{
					iter->second.validMoves.insert({ l+1, 6 });
					iter->second.validMoves.insert({ l-1, 6 });
				}
				break;



		}
	}

}

std::pair<char, int> PieceManager::getTileID(int x, int y)
{
	std::pair<char, int> result;
	result.first = x + 'a';
	result.second = 8 - y;
	return result;
}

void PieceManager::recomputeValidMoves()
{
	std::map<std::pair<char, int>, ChessPiece>::iterator iter;
	for (iter = pieces.begin(); iter != pieces.end(); iter++)
	{

		std::pair<char, int> loc = iter->second.locationNot;
		char col = loc.first;
		int row = loc.second;

		// Reset Valid Moves
		iter->second.validMoves.clear();

		// Only handle current players options
		if (turn != iter->second.playerID)
			continue;

		switch (iter->second.token)
		{
			case ChessToken::PAWN:
			{
				// Move Forward
				std::pair<char, int> tmp;
				if (iter->second.playerID)
					tmp = { col, row + 1 };
				else
					tmp = { col, row - 1 };

				if (pieces.find(tmp) == pieces.end())
					iter->second.validMoves.insert(tmp);

				// Double Move
				if (iter->second.playerID && row == 2)
				{
					if (pieces.find({ col, row + 2 }) == pieces.end())
					{
						iter->second.validMoves.insert({ col, row + 2 });
					}
				}
				else if (!iter->second.playerID && row == 7)
				{
					if (pieces.find({ col, row - 2 }) == pieces.end())
					{
						iter->second.validMoves.insert({ col, row - 2 });
					}
				}

				// Attacking
				std::pair<char, int> tmp2;
				if (iter->second.playerID)
				{
					tmp = { col - 1, row + 1 };
					tmp2 = { col + 1, row + 1 };
				}
				else
				{
					tmp = { col - 1, row - 1 };
					tmp2 = { col + 1, row - 1 };
				}
				if (pieces.find(tmp) != pieces.end() && pieces[tmp].playerID == !iter->second.playerID)
					iter->second.validMoves.insert(tmp);
				else if (pieces.find(tmp2) != pieces.end() && pieces[tmp2].playerID == !iter->second.playerID)
					iter->second.validMoves.insert(tmp2);

			}
			break;

			case ChessToken::KNIGHT:
			{
				std::pair<char, int> tmp = { col - 1, row + 2 }; // Top Left
				std::pair<char, int> tmp2 = { col + 1, row + 2 }; // Top Right

				std::pair<char, int> tmp3 = { col - 1, row - 2 }; // Bot Left
				std::pair<char, int> tmp4 = { col + 1, row - 2 }; // Bot Right

				std::pair<char, int> tmp5 = { col + 2, row + 1 }; // Right Up
				std::pair<char, int> tmp6 = { col + 2, row - 1 }; // Right Down

				std::pair<char, int> tmp7 = { col - 2, row + 1 }; // Left Up
				std::pair<char, int> tmp8 = { col - 2, row - 1 }; // Left Down

				if (pieces.find(tmp) == pieces.end() || pieces[tmp].playerID != iter->second.playerID)
					iter->second.validMoves.insert(tmp);

				if (pieces.find(tmp2) == pieces.end() || pieces[tmp2].playerID != iter->second.playerID)
					iter->second.validMoves.insert(tmp2);

				if (pieces.find(tmp3) == pieces.end() || pieces[tmp3].playerID != iter->second.playerID)
					iter->second.validMoves.insert(tmp3);

				if (pieces.find(tmp4) == pieces.end() || pieces[tmp4].playerID != iter->second.playerID)
					iter->second.validMoves.insert(tmp4);

				if (pieces.find(tmp5) == pieces.end() || pieces[tmp5].playerID != iter->second.playerID)
					iter->second.validMoves.insert(tmp5);

				if (pieces.find(tmp6) == pieces.end() || pieces[tmp6].playerID != iter->second.playerID)
					iter->second.validMoves.insert(tmp6);

				if (pieces.find(tmp7) == pieces.end() || pieces[tmp7].playerID != iter->second.playerID)
					iter->second.validMoves.insert(tmp7);

				if (pieces.find(tmp8) == pieces.end() || pieces[tmp8].playerID != iter->second.playerID)
					iter->second.validMoves.insert(tmp8);

			}
			break;

			case ChessToken::BISHOP:
			{
				char tmpCol = col;
				int tmpRow = row;

				// Top Left
				tmpCol--;
				tmpRow++;
				while (tmpCol >= 'a' && tmpCol <= 'h' && tmpRow >= 1 && tmpRow <= 8)
				{
					if (pieces.find({ tmpCol, tmpRow }) != pieces.end())
					{
						if (pieces[{tmpCol, tmpRow}].playerID != iter->second.playerID)
							iter->second.validMoves.insert({ tmpCol, tmpRow });

						break;
					}
					iter->second.validMoves.insert({ tmpCol, tmpRow });
					tmpCol--;
					tmpRow++;
				}

				// Top Right
				tmpCol = col;
				tmpRow = row;
				tmpCol++;
				tmpRow++;
				while (tmpCol >= 'a' && tmpCol <= 'h' && tmpRow >= 1 && tmpRow <= 8)
				{
					if (pieces.find({ tmpCol, tmpRow }) != pieces.end())
					{
						if (pieces[{tmpCol, tmpRow}].playerID != iter->second.playerID)
							iter->second.validMoves.insert({ tmpCol, tmpRow });

						break;
					}
					iter->second.validMoves.insert({ tmpCol, tmpRow });
					tmpCol++;
					tmpRow++;
				}

				// Bottom Right
				tmpCol = col;
				tmpRow = row;
				tmpCol++;
				tmpRow--;
				while (tmpCol >= 'a' && tmpCol <= 'h' && tmpRow >= 1 && tmpRow <= 8)
				{
					if (pieces.find({ tmpCol, tmpRow }) != pieces.end())
					{
						if (pieces[{tmpCol, tmpRow}].playerID != iter->second.playerID)
							iter->second.validMoves.insert({ tmpCol, tmpRow });

						break;
					}
					iter->second.validMoves.insert({ tmpCol, tmpRow });
					tmpCol++;
					tmpRow--;
				}

				// Bottom Left
				tmpCol = col;
				tmpRow = row;
				tmpCol--;
				tmpRow--;
				while (tmpCol >= 'a' && tmpCol <= 'h' && tmpRow >= 1 && tmpRow <= 8)
				{
					if (pieces.find({ tmpCol, tmpRow }) != pieces.end())
					{
						if (pieces[{tmpCol, tmpRow}].playerID != iter->second.playerID)
							iter->second.validMoves.insert({ tmpCol, tmpRow });

						break;
					}
					iter->second.validMoves.insert({ tmpCol, tmpRow });
					tmpCol--;
					tmpRow--;
				}


			}
			break;

			case ChessToken::ROOK:
			{
				char tmpCol = col;
				int tmpRow = row;

				// Top Movement
				tmpRow++;
				while (tmpCol >= 'a' && tmpCol <= 'h' && tmpRow >= 1 && tmpRow <= 8)
				{
					if (pieces.find({ tmpCol, tmpRow }) != pieces.end())
					{
						if (pieces[{tmpCol, tmpRow}].playerID != iter->second.playerID)
							iter->second.validMoves.insert({ tmpCol, tmpRow });

						break;
					}
					iter->second.validMoves.insert({ tmpCol, tmpRow });
					tmpRow++;
				}

				// Bot Movement
				tmpCol = col;
				tmpRow = row;
				tmpRow--;
				while (tmpCol >= 'a' && tmpCol <= 'h' && tmpRow >= 1 && tmpRow <= 8)
				{
					if (pieces.find({ tmpCol, tmpRow }) != pieces.end())
					{
						if (pieces[{tmpCol, tmpRow}].playerID != iter->second.playerID)
							iter->second.validMoves.insert({ tmpCol, tmpRow });

						break;
					}
					iter->second.validMoves.insert({ tmpCol, tmpRow });
					tmpRow--;
				}

				// Right Movement
				tmpCol = col;
				tmpRow = row;
				tmpCol++;
				while (tmpCol >= 'a' && tmpCol <= 'h' && tmpRow >= 1 && tmpRow <= 8)
				{
					if (pieces.find({ tmpCol, tmpRow }) != pieces.end())
					{
						if (pieces[{tmpCol, tmpRow}].playerID != iter->second.playerID)
							iter->second.validMoves.insert({ tmpCol, tmpRow });

						break;
					}
					iter->second.validMoves.insert({ tmpCol, tmpRow });
					tmpCol++;
				}

				// Left Movement
				tmpCol = col;
				tmpRow = row;
				tmpCol--;
				while (tmpCol >= 'a' && tmpCol <= 'h' && tmpRow >= 1 && tmpRow <= 8)
				{
					if (pieces.find({ tmpCol, tmpRow }) != pieces.end())
					{
						if (pieces[{tmpCol, tmpRow}].playerID != iter->second.playerID)
							iter->second.validMoves.insert({ tmpCol, tmpRow });

						break;
					}
					iter->second.validMoves.insert({ tmpCol, tmpRow });
					tmpCol--;
				}					
			}
			break;

			case ChessToken::QUEEN:
			{
				char tmpCol = col;
				int tmpRow = row;

				// Top Left
				tmpCol--;
				tmpRow++;
				while (tmpCol >= 'a' && tmpCol <= 'h' && tmpRow >= 1 && tmpRow <= 8)
				{
					if (pieces.find({ tmpCol, tmpRow }) != pieces.end())
					{
						if (pieces[{tmpCol, tmpRow}].playerID != iter->second.playerID)
							iter->second.validMoves.insert({ tmpCol, tmpRow });

						break;
					}
					iter->second.validMoves.insert({ tmpCol, tmpRow });
					tmpCol--;
					tmpRow++;
				}

				// Top Right
				tmpCol = col;
				tmpRow = row;
				tmpCol++;
				tmpRow++;
				while (tmpCol >= 'a' && tmpCol <= 'h' && tmpRow >= 1 && tmpRow <= 8)
				{
					if (pieces.find({ tmpCol, tmpRow }) != pieces.end())
					{
						if (pieces[{tmpCol, tmpRow}].playerID != iter->second.playerID)
							iter->second.validMoves.insert({ tmpCol, tmpRow });

						break;
					}
					iter->second.validMoves.insert({ tmpCol, tmpRow });
					tmpCol++;
					tmpRow++;
				}

				// Bottom Right
				tmpCol = col;
				tmpRow = row;
				tmpCol++;
				tmpRow--;
				while (tmpCol >= 'a' && tmpCol <= 'h' && tmpRow >= 1 && tmpRow <= 8)
				{
					if (pieces.find({ tmpCol, tmpRow }) != pieces.end())
					{
						if (pieces[{tmpCol, tmpRow}].playerID != iter->second.playerID)
							iter->second.validMoves.insert({ tmpCol, tmpRow });

						break;
					}
					iter->second.validMoves.insert({ tmpCol, tmpRow });
					tmpCol++;
					tmpRow--;
				}

				// Bottom Left
				tmpCol = col;
				tmpRow = row;
				tmpCol--;
				tmpRow--;
				while (tmpCol >= 'a' && tmpCol <= 'h' && tmpRow >= 1 && tmpRow <= 8)
				{
					if (pieces.find({ tmpCol, tmpRow }) != pieces.end())
					{
						if (pieces[{tmpCol, tmpRow}].playerID != iter->second.playerID)
							iter->second.validMoves.insert({ tmpCol, tmpRow });

						break;
					}
					iter->second.validMoves.insert({ tmpCol, tmpRow });
					tmpCol--;
					tmpRow--;
				}

				// Top Movement
				tmpCol = col;
				tmpRow = row;
				tmpRow++;
				while (tmpCol >= 'a' && tmpCol <= 'h' && tmpRow >= 1 && tmpRow <= 8)
				{
					if (pieces.find({ tmpCol, tmpRow }) != pieces.end())
					{
						if (pieces[{tmpCol, tmpRow}].playerID != iter->second.playerID)
							iter->second.validMoves.insert({ tmpCol, tmpRow });

						break;
					}
					iter->second.validMoves.insert({ tmpCol, tmpRow });
					tmpRow++;
				}

				// Bot Movement
				tmpCol = col;
				tmpRow = row;
				tmpRow--;
				while (tmpCol >= 'a' && tmpCol <= 'h' && tmpRow >= 1 && tmpRow <= 8)
				{
					if (pieces.find({ tmpCol, tmpRow }) != pieces.end())
					{
						if (pieces[{tmpCol, tmpRow}].playerID != iter->second.playerID)
							iter->second.validMoves.insert({ tmpCol, tmpRow });

						break;
					}
					iter->second.validMoves.insert({ tmpCol, tmpRow });
					tmpRow--;
				}

				// Right Movement
				tmpCol = col;
				tmpRow = row;
				tmpCol++;
				while (tmpCol >= 'a' && tmpCol <= 'h' && tmpRow >= 1 && tmpRow <= 8)
				{
					if (pieces.find({ tmpCol, tmpRow }) != pieces.end())
					{
						if (pieces[{tmpCol, tmpRow}].playerID != iter->second.playerID)
							iter->second.validMoves.insert({ tmpCol, tmpRow });

						break;
					}
					iter->second.validMoves.insert({ tmpCol, tmpRow });
					tmpCol++;
				}

				// Left Movement
				tmpCol = col;
				tmpRow = row;
				tmpCol--;
				while (tmpCol >= 'a' && tmpCol <= 'h' && tmpRow >= 1 && tmpRow <= 8)
				{
					if (pieces.find({ tmpCol, tmpRow }) != pieces.end())
					{
						if (pieces[{tmpCol, tmpRow}].playerID != iter->second.playerID)
							iter->second.validMoves.insert({ tmpCol, tmpRow });

						break;
					}
					iter->second.validMoves.insert({ tmpCol, tmpRow });
					tmpCol--;
				}



			}
			break;

			case ChessToken::KING:
			{
				std::pair<char, int> tmp = { col, row + 1};       // N Movement
				std::pair<char, int> tmp2 = { col + 1, row + 1 }; // NE Movement
				std::pair<char, int> tmp3 = { col + 1, row };     // E Movement
				std::pair<char, int> tmp4 = { col + 1, row - 1 }; // SE Movement
				std::pair<char, int> tmp5 = { col, row - 1};      // S Movement
				std::pair<char, int> tmp6 = { col - 1, row - 1 }; // SW Movement
				std::pair<char, int> tmp7 = { col - 1, row};      // W Movement
				std::pair<char, int> tmp8 = { col - 1, row + 1 }; // NW Movement

				if (tmp.first >= 'a' && tmp.first <= 'h' && tmp.second >= 1 && tmp.second <= 8)
				{
					if (pieces.find(tmp) == pieces.end() || pieces[tmp].playerID != iter->second.playerID)
						iter->second.validMoves.insert(tmp);
				}

				if (tmp2.first >= 'a' && tmp2.first <= 'h' && tmp2.second >= 1 && tmp2.second <= 8)
				{
					if (pieces.find(tmp2) == pieces.end() || pieces[tmp2].playerID != iter->second.playerID)
						iter->second.validMoves.insert(tmp2);
				}

				if (tmp3.first >= 'a' && tmp3.first <= 'h' && tmp3.second >= 1 && tmp3.second <= 8)
				{
					if (pieces.find(tmp3) == pieces.end() || pieces[tmp3].playerID != iter->second.playerID)
						iter->second.validMoves.insert(tmp3);
				}

				if (tmp4.first >= 'a' && tmp4.first <= 'h' && tmp4.second >= 1 && tmp4.second <= 8)
				{
					if (pieces.find(tmp4) == pieces.end() || pieces[tmp4].playerID != iter->second.playerID)
						iter->second.validMoves.insert(tmp4);
				}

				if (tmp5.first >= 'a' && tmp5.first <= 'h' && tmp5.second >= 1 && tmp5.second <= 8)
				{
					if (pieces.find(tmp5) == pieces.end() || pieces[tmp5].playerID != iter->second.playerID)
						iter->second.validMoves.insert(tmp5);
				}

				if (tmp6.first >= 'a' && tmp6.first <= 'h' && tmp6.second >= 1 && tmp6.second <= 8)
				{
					if (pieces.find(tmp6) == pieces.end() || pieces[tmp6].playerID != iter->second.playerID)
						iter->second.validMoves.insert(tmp6);
				}

				if (tmp7.first >= 'a' && tmp7.first <= 'h' && tmp7.second >= 1 && tmp7.second <= 8)
				{
					if (pieces.find(tmp7) == pieces.end() || pieces[tmp7].playerID != iter->second.playerID)
						iter->second.validMoves.insert(tmp7);
				}

				if (tmp8.first >= 'a' && tmp8.first <= 'h' && tmp8.second >= 1 && tmp8.second <= 8)
				{
					if (pieces.find(tmp8) == pieces.end() || pieces[tmp8].playerID != iter->second.playerID)
						iter->second.validMoves.insert(tmp8);
				}


			}
			break;
		}

		// Check King Safety 
		std::set<std::pair<char, int>> tmpValid = iter->second.validMoves;

		std::set<std::pair<char, int>>::iterator iter2;
		for (iter2 = tmpValid.begin(); iter2 != tmpValid.end(); iter2++)
		{
			if (!kingSafe({ col,row }, *iter2))
			{				
				iter->second.validMoves.erase(*iter2);
			}


		}


	}

}

bool PieceManager::kingSafe(std::pair<char, int> src, std::pair<char, int> dst)
{
	// Simulate Movement to see if Valid
	std::map<std::pair<char, int>, ChessPiece> pieceCopy = pieces;
	pieceCopy.erase(dst);
	pieceCopy[dst] = pieces[src];
	pieceCopy.erase(src);
	pieceCopy[dst].locationNot = dst;

	// See if King of current turn is safe or not

	std::map<std::pair<char, int>, ChessPiece>::iterator iter;
	for (iter = pieces.begin(); iter != pieces.end(); iter++)
	{
		// Only handle current players options
		if (turn == iter->second.playerID)
			continue;

		std::pair<char, int> loc = iter->second.locationNot;
		char col = loc.first;
		int row = loc.second;

		switch (iter->second.token)
		{
			case ChessToken::PAWN:
			{

				// Attacking
				std::pair<char, int> tmp;
				std::pair<char, int> tmp2;
				if (iter->second.playerID)
				{
					tmp = { col - 1, row + 1 };
					tmp2 = { col + 1, row + 1 };
				}
				else
				{
					tmp = { col - 1, row - 1 };
					tmp2 = { col + 1, row - 1 };
				}


				if (pieceCopy.find(tmp) != pieceCopy.end() && pieceCopy[tmp].playerID == !iter->second.playerID)
				{
					if (pieceCopy[tmp].token == ChessToken::KING)
						return false;
				}
				else if (pieceCopy.find(tmp2) != pieceCopy.end() && pieceCopy[tmp2].playerID == !iter->second.playerID)
				{
					if (pieceCopy[tmp2].token == ChessToken::KING)
						return false;
				}

			}
			break;

			case ChessToken::KNIGHT:
			{
				std::pair<char, int> tmp = { col - 1, row + 2 }; // Top Left
				std::pair<char, int> tmp2 = { col + 1, row + 2 }; // Top Right

				std::pair<char, int> tmp3 = { col - 1, row - 2 }; // Bot Left
				std::pair<char, int> tmp4 = { col + 1, row - 2 }; // Bot Right

				std::pair<char, int> tmp5 = { col + 2, row + 1 }; // Right Up
				std::pair<char, int> tmp6 = { col + 2, row - 1 }; // Right Down

				std::pair<char, int> tmp7 = { col - 2, row + 1 }; // Left Up
				std::pair<char, int> tmp8 = { col - 2, row - 1 }; // Left Down

				if (pieceCopy.find(tmp) != pieceCopy.end() && pieceCopy[tmp].playerID != iter->second.playerID)
				{
					if (pieceCopy[tmp].token == ChessToken::KING)
						return false;
				}

				if (pieceCopy.find(tmp2) != pieceCopy.end() && pieceCopy[tmp2].playerID != iter->second.playerID)
				{
					if (pieceCopy[tmp2].token == ChessToken::KING)
						return false;
				}

				if (pieceCopy.find(tmp3) != pieceCopy.end() && pieceCopy[tmp3].playerID != iter->second.playerID)
				{
					if (pieceCopy[tmp3].token == ChessToken::KING)
						return false;
				}

				if (pieceCopy.find(tmp4) != pieceCopy.end() && pieceCopy[tmp4].playerID != iter->second.playerID)
				{
					if (pieceCopy[tmp4].token == ChessToken::KING)
						return false;
				}

				if (pieceCopy.find(tmp5) != pieceCopy.end() && pieceCopy[tmp5].playerID != iter->second.playerID)
				{
					if (pieceCopy[tmp5].token == ChessToken::KING)
						return false;
				}

				if (pieceCopy.find(tmp6) != pieceCopy.end() && pieceCopy[tmp6].playerID != iter->second.playerID)
				{
					if (pieceCopy[tmp6].token == ChessToken::KING)
						return false;
				}

				if (pieceCopy.find(tmp7) != pieceCopy.end() && pieceCopy[tmp7].playerID != iter->second.playerID)
				{
					if (pieceCopy[tmp7].token == ChessToken::KING)
						return false;
				}

				if (pieceCopy.find(tmp8) != pieceCopy.end() && pieceCopy[tmp8].playerID != iter->second.playerID)
				{
					if (pieceCopy[tmp8].token == ChessToken::KING)
						return false;
				}



			}
			break;

			case ChessToken::BISHOP:
			{
				char tmpCol = col;
				int tmpRow = row;

				// Top Left
				tmpCol--;
				tmpRow++;
				while (tmpCol >= 'a' && tmpCol <= 'h' && tmpRow >= 1 && tmpRow <= 8)
				{
					if (pieceCopy.find({ tmpCol, tmpRow }) != pieceCopy.end())
					{
						if (pieceCopy[{tmpCol, tmpRow}].playerID != iter->second.playerID)
						{
							if (pieceCopy[{tmpCol, tmpRow}].token == ChessToken::KING)
								return false;
						}
						break;
					}
					tmpCol--;
					tmpRow++;
				}

				// Top Right
				tmpCol = col;
				tmpRow = row;
				tmpCol++;
				tmpRow++;
				while (tmpCol >= 'a' && tmpCol <= 'h' && tmpRow >= 1 && tmpRow <= 8)
				{
					if (pieceCopy.find({ tmpCol, tmpRow }) != pieceCopy.end())
					{
						if (pieceCopy[{tmpCol, tmpRow}].playerID != iter->second.playerID)
						{
							if (pieceCopy[{tmpCol, tmpRow}].token == ChessToken::KING)
								return false;
						}

						break;
					}
					tmpCol++;
					tmpRow++;
				}

				// Bottom Right
				tmpCol = col;
				tmpRow = row;
				tmpCol++;
				tmpRow--;
				while (tmpCol >= 'a' && tmpCol <= 'h' && tmpRow >= 1 && tmpRow <= 8)
				{
					if (pieceCopy.find({ tmpCol, tmpRow }) != pieceCopy.end())
					{
						if (pieceCopy[{tmpCol, tmpRow}].playerID != iter->second.playerID)
						{
							if (pieceCopy[{tmpCol, tmpRow}].token == ChessToken::KING)
								return false;
						}

						break;
					}
					tmpCol++;
					tmpRow--;
				}

				// Bottom Left
				tmpCol = col;
				tmpRow = row;
				tmpCol--;
				tmpRow--;
				while (tmpCol >= 'a' && tmpCol <= 'h' && tmpRow >= 1 && tmpRow <= 8)
				{
					if (pieceCopy.find({ tmpCol, tmpRow }) != pieceCopy.end())
					{
						if (pieceCopy[{tmpCol, tmpRow}].playerID != iter->second.playerID)
						{
							if (pieceCopy[{tmpCol, tmpRow}].token == ChessToken::KING)
								return false;
						}

						break;
					}
					tmpCol--;
					tmpRow--;
				}


			}
			break;

			case ChessToken::ROOK:
			{
				char tmpCol = col;
				int tmpRow = row;

				// Top Movement
				tmpRow++;
				while (tmpCol >= 'a' && tmpCol <= 'h' && tmpRow >= 1 && tmpRow <= 8)
				{
					if (pieceCopy.find({ tmpCol, tmpRow }) != pieceCopy.end())
					{
						if (pieceCopy[{tmpCol, tmpRow}].playerID != iter->second.playerID)
						{
							if (pieceCopy[{tmpCol, tmpRow}].token == ChessToken::KING)
								return false;
						}
						break;
					}
					tmpRow++;
				}

				// Bot Movement
				tmpCol = col;
				tmpRow = row;
				tmpRow--;
				while (tmpCol >= 'a' && tmpCol <= 'h' && tmpRow >= 1 && tmpRow <= 8)
				{
					if (pieceCopy.find({ tmpCol, tmpRow }) != pieceCopy.end())
					{
						if (pieceCopy[{tmpCol, tmpRow}].playerID != iter->second.playerID)
						{
							if (pieceCopy[{tmpCol, tmpRow}].token == ChessToken::KING)
								return false;
						}

						break;
					}
					tmpRow--;
				}

				// Right Movement
				tmpCol = col;
				tmpRow = row;
				tmpCol++;
				while (tmpCol >= 'a' && tmpCol <= 'h' && tmpRow >= 1 && tmpRow <= 8)
				{
					if (pieceCopy.find({ tmpCol, tmpRow }) != pieceCopy.end())
					{
						if (pieceCopy[{tmpCol, tmpRow}].playerID != iter->second.playerID)
						{
							if (pieceCopy[{tmpCol, tmpRow}].token == ChessToken::KING)
								return false;
						}

						break;
					}
					tmpCol++;
				}

				// Left Movement
				tmpCol = col;
				tmpRow = row;
				tmpCol--;
				while (tmpCol >= 'a' && tmpCol <= 'h' && tmpRow >= 1 && tmpRow <= 8)
				{
					if (pieceCopy.find({ tmpCol, tmpRow }) != pieceCopy.end())
					{
						if (pieceCopy[{tmpCol, tmpRow}].playerID != iter->second.playerID)
						{
							if (pieceCopy[{tmpCol, tmpRow}].token == ChessToken::KING)
								return false;
						}

						break;
					}
					tmpCol--;
				}
			}
			break;

			case ChessToken::QUEEN:
			{
				char tmpCol = col;
				int tmpRow = row;

				// Top Left
				tmpCol--;
				tmpRow++;
				while (tmpCol >= 'a' && tmpCol <= 'h' && tmpRow >= 1 && tmpRow <= 8)
				{
					if (pieceCopy.find({ tmpCol, tmpRow }) != pieceCopy.end())
					{
						if (pieceCopy[{tmpCol, tmpRow}].playerID != iter->second.playerID)
						{
							if (pieceCopy[{tmpCol, tmpRow}].token == ChessToken::KING)
								return false;
						}

						break;
					}
					tmpCol--;
					tmpRow++;
				}

				// Top Right
				tmpCol = col;
				tmpRow = row;
				tmpCol++;
				tmpRow++;
				while (tmpCol >= 'a' && tmpCol <= 'h' && tmpRow >= 1 && tmpRow <= 8)
				{
					if (pieceCopy.find({ tmpCol, tmpRow }) != pieceCopy.end())
					{
						if (pieceCopy[{tmpCol, tmpRow}].playerID != iter->second.playerID)
						{
							if (pieceCopy[{tmpCol, tmpRow}].token == ChessToken::KING)
								return false;
						}

						break;
					}
					tmpCol++;
					tmpRow++;
				}

				// Bottom Right
				tmpCol = col;
				tmpRow = row;
				tmpCol++;
				tmpRow--;
				while (tmpCol >= 'a' && tmpCol <= 'h' && tmpRow >= 1 && tmpRow <= 8)
				{
					if (pieceCopy.find({ tmpCol, tmpRow }) != pieceCopy.end())
					{
						if (pieceCopy[{tmpCol, tmpRow}].playerID != iter->second.playerID)
						{
							if (pieceCopy[{tmpCol, tmpRow}].token == ChessToken::KING)
								return false;
						}

						break;
					}
					tmpCol++;
					tmpRow--;
				}

				// Bottom Left
				tmpCol = col;
				tmpRow = row;
				tmpCol--;
				tmpRow--;
				while (tmpCol >= 'a' && tmpCol <= 'h' && tmpRow >= 1 && tmpRow <= 8)
				{
					if (pieceCopy.find({ tmpCol, tmpRow }) != pieceCopy.end())
					{
						if (pieceCopy[{tmpCol, tmpRow}].playerID != iter->second.playerID)
						{
							if (pieceCopy[{tmpCol, tmpRow}].token == ChessToken::KING)
								return false;
						}

						break;
					}
					tmpCol--;
					tmpRow--;
				}

				// Top Movement
				tmpCol = col;
				tmpRow = row;
				tmpRow++;
				while (tmpCol >= 'a' && tmpCol <= 'h' && tmpRow >= 1 && tmpRow <= 8)
				{
					if (pieceCopy.find({ tmpCol, tmpRow }) != pieceCopy.end())
					{
						if (pieceCopy[{tmpCol, tmpRow}].playerID != iter->second.playerID)
						{
							if (pieceCopy[{tmpCol, tmpRow}].token == ChessToken::KING)
								return false;
						}

						break;
					}
					tmpRow++;
				}

				// Bot Movement
				tmpCol = col;
				tmpRow = row;
				tmpRow--;
				while (tmpCol >= 'a' && tmpCol <= 'h' && tmpRow >= 1 && tmpRow <= 8)
				{
					if (pieceCopy.find({ tmpCol, tmpRow }) != pieceCopy.end())
					{
						if (pieceCopy[{tmpCol, tmpRow}].playerID != iter->second.playerID)
						{
							if (pieceCopy[{tmpCol, tmpRow}].token == ChessToken::KING)
								return false;
						}

						break;
					}
					tmpRow--;
				}

				// Right Movement
				tmpCol = col;
				tmpRow = row;
				tmpCol++;
				while (tmpCol >= 'a' && tmpCol <= 'h' && tmpRow >= 1 && tmpRow <= 8)
				{
					if (pieceCopy.find({ tmpCol, tmpRow }) != pieceCopy.end())
					{
						if (pieceCopy[{tmpCol, tmpRow}].playerID != iter->second.playerID)
						{
							if (pieceCopy[{tmpCol, tmpRow}].token == ChessToken::KING)
								return false;
						}

						break;
					}
					tmpCol++;
				}

				// Left Movement
				tmpCol = col;
				tmpRow = row;
				tmpCol--;
				while (tmpCol >= 'a' && tmpCol <= 'h' && tmpRow >= 1 && tmpRow <= 8)
				{
					if (pieceCopy.find({ tmpCol, tmpRow }) != pieceCopy.end())
					{
						if (pieceCopy[{tmpCol, tmpRow}].playerID != iter->second.playerID)
						{
							if (pieceCopy[{tmpCol, tmpRow}].token == ChessToken::KING)
								return false;
						}

						break;
					}
					tmpCol--;
				}



			}
			break;

			case ChessToken::KING:
			 {
				std::pair<char, int> tmp = { col, row + 1 };       // N Movement
				std::pair<char, int> tmp2 = { col + 1, row + 1 }; // NE Movement
				std::pair<char, int> tmp3 = { col + 1, row };     // E Movement
				std::pair<char, int> tmp4 = { col + 1, row - 1 }; // SE Movement
				std::pair<char, int> tmp5 = { col, row - 1 };      // S Movement
				std::pair<char, int> tmp6 = { col - 1, row - 1 }; // SW Movement
				std::pair<char, int> tmp7 = { col - 1, row };      // W Movement
				std::pair<char, int> tmp8 = { col - 1, row + 1 }; // NW Movement

				if (pieceCopy.find(tmp) != pieceCopy.end() && pieceCopy[tmp].playerID != iter->second.playerID)
				{
					if (pieceCopy[tmp].token == ChessToken::KING)
						return false;
				}

				if (pieceCopy.find(tmp2) != pieceCopy.end() && pieceCopy[tmp2].playerID != iter->second.playerID)
				{
					if (pieceCopy[tmp2].token == ChessToken::KING)
						return false;
				}

				if (pieceCopy.find(tmp3) != pieceCopy.end() && pieceCopy[tmp3].playerID != iter->second.playerID)
				{
					if (pieceCopy[tmp3].token == ChessToken::KING)
						return false;
				}

				if (pieceCopy.find(tmp4) != pieceCopy.end() && pieceCopy[tmp4].playerID != iter->second.playerID)
				{
					if (pieceCopy[tmp4].token == ChessToken::KING)
						return false;
				}

				if (pieceCopy.find(tmp5) != pieceCopy.end() && pieceCopy[tmp5].playerID != iter->second.playerID)
				{
					if (pieceCopy[tmp5].token == ChessToken::KING)
						return false;
				}

				if (pieceCopy.find(tmp6) != pieceCopy.end() && pieceCopy[tmp6].playerID != iter->second.playerID)
				{
					if (pieceCopy[tmp6].token == ChessToken::KING)
						return false;
				}

				if (pieceCopy.find(tmp7) != pieceCopy.end() && pieceCopy[tmp7].playerID != iter->second.playerID)
				{
					if (pieceCopy[tmp7].token == ChessToken::KING)
						return false;
				}

				if (pieceCopy.find(tmp8) != pieceCopy.end() && pieceCopy[tmp8].playerID != iter->second.playerID)
				{
					if (pieceCopy[tmp8].token == ChessToken::KING)
						return false;
				}






			}
			break;


		}
	}

	return true;
}

bool PieceManager::checkmateChecker()
{
	std::map<std::pair<char, int>, ChessPiece>::iterator iter;
	for (iter = pieces.begin(); iter != pieces.end(); iter++)
	{
		if (iter->second.validMoves.size() > 0)
		{
			return false;
		}

	}


	return true;
}