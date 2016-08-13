// 4Chess.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

const int SCREEN_WIDTH = 512;
const int SCREEN_HEIGHT = 512;
const int TokenXSize = 64;
const int TokenYSize = 64;

bool w_7RookMoved = false;
bool w_0RookMoved = false;
bool b_7RookMoved = false;
bool b_0RookMoved = false;
bool w_KingMoved = false;
bool b_KingMoved = false;

string InitPosition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
string* StandardInitPosition = new string;

SDL_Surface* gScreenSurface = NULL;

//Surface del tablero
SDL_Surface* gBoardSurface = NULL;
//piezas del jugador blanco
SDL_Surface* gWhite_Player[SURFACE_TOTAL];
//piezas del jugador negro
SDL_Surface* gBlack_Player[SURFACE_TOTAL];

SDL_Surface* gHighlighted;

SDL_Surface* gAvailable;

SDL_Window* gWindow = NULL;

SDL_Surface* loadSurface(std::string path)
{
	////Load image at specified path
	SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
	}
	return loadedSurface;
}

bool init()
{
	IMG_Init(IMG_INIT_PNG);
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow("Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Get window surface
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}
	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;
	//Load default surface
	//cada imagen es de 32x32, el tablero debera ser de 256x256

	gBoardSurface = IMG_Load("Tokens/board.png");
	if (gBoardSurface == NULL)
	{
		printf("Failed to load board image!\n");
		success = false;
	}

	gHighlighted = IMG_Load("Tokens/Highlight.png");
	if (gHighlighted == NULL)
	{
		printf("Failed to load highlighted image!\n");
		success = false;
	}

	gAvailable = IMG_Load("Tokens/Available.png");
	if (gAvailable == NULL)
	{
		printf("Failed to load available image!\n");
		success = false;
	}

	gWhite_Player[TOKEN_PAWN] = IMG_Load("Tokens/pawn.png");
	if (gWhite_Player[TOKEN_PAWN] == NULL)
	{
		printf("Failed to load pawn image!\n");
		success = false;
	}

	gWhite_Player[TOKEN_BISHOP] = IMG_Load("Tokens/bishop.png");
	if (gWhite_Player[TOKEN_PAWN] == NULL)
	{
		printf("Failed to load bishop image!\n");
		success = false;
	}

	gWhite_Player[TOKEN_ROOK] = IMG_Load("Tokens/rook.png");
	if (gWhite_Player[TOKEN_ROOK] == NULL)
	{
		printf("Failed to load rook image!\n");
		success = false;
	}

	gWhite_Player[TOKEN_KNIGHT] = IMG_Load("Tokens/knight.png");
	if (gWhite_Player[TOKEN_KNIGHT] == NULL)
	{
		printf("Failed to load knight image!\n");
		success = false;
	}

	gWhite_Player[TOKEN_QUEEN] = IMG_Load("Tokens/queen.png");
	if (gWhite_Player[TOKEN_QUEEN] == NULL)
	{
		printf("Failed to load queen image!\n");
		success = false;
	}

	gWhite_Player[TOKEN_KING] = IMG_Load("Tokens/king.png");
	if (gWhite_Player[TOKEN_KING] == NULL)
	{
		printf("Failed to load king image!\n");
		success = false;
	}

	gBlack_Player[TOKEN_PAWN] = IMG_Load("Tokens/b_pawn.png");
	if (gBlack_Player[TOKEN_PAWN] == NULL)
	{
		printf("Failed to load bpawn image!\n");
		success = false;
	}

	gBlack_Player[TOKEN_BISHOP] = IMG_Load("Tokens/b_bishop.png");
	if (gBlack_Player[TOKEN_PAWN] == NULL)
	{
		printf("Failed to load bbishop image!\n");
		success = false;
	}

	gBlack_Player[TOKEN_ROOK] = IMG_Load("Tokens/b_rook.png");
	if (gBlack_Player[TOKEN_ROOK] == NULL)
	{
		printf("Failed to load brook image!\n");
		success = false;
	}

	gBlack_Player[TOKEN_KNIGHT] = IMG_Load("Tokens/b_knight.png");
	if (gBlack_Player[TOKEN_KNIGHT] == NULL)
	{
		printf("Failed to load bknight image!\n");
		success = false;
	}

	gBlack_Player[TOKEN_QUEEN] = IMG_Load("Tokens/b_queen.png");
	if (gBlack_Player[TOKEN_QUEEN] == NULL)
	{
		printf("Failed to load bqueen image!\n");
		success = false;
	}

	gBlack_Player[TOKEN_KING] = IMG_Load("Tokens/b_king.png");
	if (gBlack_Player[TOKEN_KING] == NULL)
	{
		printf("Failed to load bking image!\n");
		success = false;
	}

	return success;
}

void close()
{
	IMG_Quit();

	for (int i = 0; i < SURFACE_TOTAL; ++i)
	{
		SDL_FreeSurface(gBlack_Player[i]);
		gBlack_Player[i] = NULL;
	}

	for (int i = 0; i < SURFACE_TOTAL; ++i)
	{
		SDL_FreeSurface(gWhite_Player[i]);
		gWhite_Player[i] = NULL;
	}
	gBoardSurface = NULL;
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	SDL_Quit();
}

int main(int argc, char* args[])
{
	bool quit = false;
	int MousePositionX;
	int MousePositionY;
	int ClickedCol;
	int ClickedRow;


	char SelectedToken = '1';
	int SelectedTokenCol = 0;
	int SelectedTokenRow = 0;


	if (!isValidInput(InitPosition))
	{
		printf("Estructura incorrecta \n");
		quit = true;
	}
	if (!(init() && loadMedia()))
	{
		printf("Error al cargar las librerias SDL \n");
	}
	else
	{
		SDL_Event e;

		MakeStandardFormat(InitPosition);

		while (!quit)
		{
			if (isThereCheckAt(FindKing(true), true, true))
			{
				cout << "Check en Rey Blanco" << "\n";
			}
			if (isThereCheckAt(FindKing(false), false, true))
			{
				cout << "Check en Rey Negro" << "\n";
			}

			while (SDL_PollEvent(&e) != 0)
			{
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
				if ((e.type == SDL_MOUSEBUTTONDOWN) && (e.button.button == SDL_BUTTON_LEFT))
				{
					SDL_GetMouseState(&MousePositionX, &MousePositionY);
					GetBoardColRow(MousePositionX, MousePositionY, ClickedCol, ClickedRow);

					if (SelectedToken == '1')
					{
						SelectedToken = GetTokenChar(ClickedCol, ClickedRow);
						SelectedTokenCol = ClickedCol;
						SelectedTokenRow = ClickedRow;
					}
					else
					{
						if (isCastlingValid(SelectedToken, ClickedCol, ClickedRow))
						{
							SelectedToken = '1';
						}
						else

							if (isValidMove(SelectedToken, SelectedTokenCol, SelectedTokenRow, ClickedCol, ClickedRow))
							{

								//solo borra el peon de costado si a donde esta moviendose esta vacio
								if ((SelectedToken == 'P') && (GetTokenChar(ClickedCol, ClickedRow) == '1'))
								{ 
									RemoveToken(ClickedCol, ClickedRow + 1); 
								}								
								if ((SelectedToken == 'p') && (GetTokenChar(ClickedCol, ClickedRow) == '1'))
								{ 
									RemoveToken(ClickedCol, ClickedRow - 1); 
								}

								if (((SelectedToken == 'K') && !(isThereCheckAt(ClickedCol + ClickedRow * 8, true, false))) ||
									(SelectedToken == 'k') && !(isThereCheckAt(ClickedCol + ClickedRow * 8, false, false)) ||
									!((SelectedToken == 'K') || (SelectedToken == 'k')))
								{
									RemoveToken(SelectedTokenCol, SelectedTokenRow);
									AddToken(SelectedToken, ClickedCol, ClickedRow);
								}

								//Castling bools
								if (SelectedToken == 'K') { w_KingMoved = true; }
								if (SelectedToken == 'k') { b_KingMoved = true; }
								if (SelectedTokenCol == 0)
								{
									if (SelectedToken == 'r') { b_0RookMoved = true; }
									if (SelectedToken == 'R') { w_0RookMoved = true; }
								}
								if (SelectedTokenCol == 7)
								{
									if (SelectedToken == 'r') { b_7RookMoved = true; }
									if (SelectedToken == 'R') { w_7RookMoved = true; }
								}
								SelectedToken = '1';
							}
							else
							{
								SelectedToken = '1';

							}
					}
				}
			}

			SDL_BlitSurface(gBoardSurface, NULL, gScreenSurface, NULL);
			drawChessBoard();
			if (SelectedToken != '1')
			{
				SetSurfacePosition(ClickedCol * TokenXSize, ClickedRow * TokenYSize, gHighlighted, gScreenSurface);
			}
			ShowAvailablePositions(SelectedToken, SelectedTokenCol, SelectedTokenRow);
			SDL_UpdateWindowSurface(gWindow);
		}
	}

	close();
	return 0;
}

void SetSurfacePosition(int x, int y, SDL_Surface* Surface, SDL_Surface* Destination)
{
	SDL_Rect Rect;

	Rect.x = x;
	Rect.y = y;

	SDL_BlitSurface(Surface, NULL, Destination, &Rect);
}

void drawChessBoard()
{
	for (int i_row = 0; i_row < 8; i_row++)
	{
		for (int i_col = 0; i_col < 8; i_col++)
		{
				switch ((int)GetTokenChar(i_col,i_row))
				{
				case 66:
					SetSurfacePosition(i_col * TokenXSize, i_row * TokenYSize, gWhite_Player[TOKEN_BISHOP], gScreenSurface);
					break;
				case 75:
					SetSurfacePosition(i_col * TokenXSize, i_row * TokenYSize, gWhite_Player[TOKEN_KING], gScreenSurface);
					break;
				case 78:
					SetSurfacePosition(i_col * TokenXSize, i_row * TokenYSize, gWhite_Player[TOKEN_KNIGHT], gScreenSurface);
					break;
				case 80:
					SetSurfacePosition(i_col * TokenXSize, i_row * TokenYSize, gWhite_Player[TOKEN_PAWN], gScreenSurface);
					break;
				case 81:
					SetSurfacePosition(i_col * TokenXSize, i_row * TokenYSize, gWhite_Player[TOKEN_QUEEN], gScreenSurface);
					break;
				case 82:
					SetSurfacePosition(i_col * TokenXSize, i_row * TokenYSize, gWhite_Player[TOKEN_ROOK], gScreenSurface);
					break;
				case 98:
					SetSurfacePosition(i_col * TokenXSize, i_row * TokenYSize, gBlack_Player[TOKEN_BISHOP], gScreenSurface);
					break;
				case 107:
					SetSurfacePosition(i_col * TokenXSize, i_row * TokenYSize, gBlack_Player[TOKEN_KING], gScreenSurface);
					break;
				case 110:
					SetSurfacePosition(i_col * TokenXSize, i_row * TokenYSize, gBlack_Player[TOKEN_KNIGHT], gScreenSurface);
					break;
				case 112:
					SetSurfacePosition(i_col * TokenXSize, i_row * TokenYSize, gBlack_Player[TOKEN_PAWN], gScreenSurface);
					break;
				case 113:
					SetSurfacePosition(i_col * TokenXSize, i_row * TokenYSize, gBlack_Player[TOKEN_QUEEN], gScreenSurface);
					break;
				case 114:
					SetSurfacePosition(i_col * TokenXSize, i_row * TokenYSize, gBlack_Player[TOKEN_ROOK], gScreenSurface);
					break;
				}
		}
	}
}

bool isValidInput(string InitPosition)
{
	///se considera que los peones pueden haber coronado, teniendo mas piezas de otros tipos. solo se revisa que el string tenga el tamanio y formato correcto
	int kCounter = 0;
	int pCounter = 0;

	int KCounter = 0;
	int PCounter = 0;

	unsigned Cursor = 0;
	int Row = 0;
	int LineXPosition;
	///loop que se repite hasta terminar el string
	while (Cursor < (InitPosition.length()))
	{
		LineXPosition = 0;
		///loop que obtiene cada fila
		while ((Cursor < (InitPosition.length())) && (InitPosition.at(Cursor) != '/'))
		{
			switch ((int)InitPosition.at(Cursor))
			{
			case 75:
				KCounter++;
				break;
			case 80:
				PCounter++;
				break;
			case 107:
				kCounter++;
				break;
			case 112:
				pCounter++;
				break;
			}
			if (((int)InitPosition.at(Cursor) > 48) && (int)InitPosition.at(Cursor) < 58) { LineXPosition += ((int)InitPosition.at(Cursor) - 48); } 
			else { LineXPosition++; }
			Cursor++;
		}
		Row++;
		Cursor++;
		if (LineXPosition != 8) { return false; }
	}
	if (Row != 8) { return false; }
	return ((KCounter<2)&&(kCounter<2)&&(PCounter<9)&&(pCounter<9));
}

void GetBoardColRow(int InputX, int InputY, int &Col, int &Row)
{
	//los valores iran de 0 a 7
	Col = InputX / TokenXSize;
	Row = InputY / TokenYSize;
}

char GetTokenChar(int Col, int Row)
{
	return StandardInitPosition->at(Row * 8 + Col);
}

void AddToken(char Token, int Col, int Row)
{
	StandardInitPosition->at(Row * 8 + Col) = Token;
}

char RemoveToken(int Col, int Row)
{
	char RemovedToken = GetTokenChar(Col, Row);
	AddToken('1', Col, Row);
	return RemovedToken;
}

void MakeStandardFormat(string InitPosition)
{
	int Cursor = 0;
	while (Cursor != InitPosition.length())
	{
		if (InitPosition.at(Cursor) != '/')
		{
			if (((int)InitPosition.at(Cursor) > 48) && ((int)InitPosition.at(Cursor) < 58))
			{
				for (int i = 0; i < ((int)InitPosition.at(Cursor) - 48); i++)
				{					
					*StandardInitPosition += "1";
				}
			}
			else
			{
				*StandardInitPosition += InitPosition.at(Cursor);
			}
		}
		Cursor++;
	}
}

bool isValidMove(char Token, int Col, int Row, int ToMoveCol, int ToMoveRow)
{
	//primero se calcularan los espacios a donde puede ir la pieza, luego se revisara que esten vacios y que no hayan fichas bloqueando
	//el tamanio del array depende de la ficha con mayor rango y no cuenta la posicion donde se encuentra actualmente.
	int* AvailablePos = new int[28];

	CalculateAvailablePos(AvailablePos, Token, Col, Row, true);
	if (isCandidateContained(AvailablePos, ToMoveCol, ToMoveRow)) { return true; }
	return false;
}

void BishopCandidatePos(int AvailablePos[], int Col, int Row, bool isWhite)
{
	int CandidatesIndex = 0;
	int CurrentCol;
	int CurrentRow;

	CurrentCol = Col + 1;
	CurrentRow = Row + 1;
	while ((CurrentCol < 8) && (CurrentRow < 8))
	{
		//si esta vacio, se agrega, si no, se deja de buscar
		if (StandardInitPosition->at(CurrentCol+CurrentRow*8) == '1')
		{
			AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow*8;
			CandidatesIndex++;
			CurrentCol++;
			CurrentRow++;
		}
		else
		{
			if (isWhite ? (isBlackToken(CurrentCol, CurrentRow)): (isWhiteToken(CurrentCol, CurrentRow)))
			{
				AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
				CandidatesIndex++;
				break;
			}
			break;
		}
	}
	CurrentCol = Col - 1;
	CurrentRow = Row - 1;
	while ((CurrentCol >= 0) && (CurrentRow >= 0))
	{
		if (StandardInitPosition->at(CurrentCol+CurrentRow*8) == '1')
		{
			AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
			CandidatesIndex++;
			CurrentCol--;
			CurrentRow--;
		}
		else
		{
			if (isWhite ? (isBlackToken(CurrentCol, CurrentRow)) : (isWhiteToken(CurrentCol, CurrentRow)))
			{
				AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
				CandidatesIndex++;
				break;
			}
			break;
		}
	}

	CurrentCol = Col - 1;
	CurrentRow = Row + 1;
	while ((CurrentCol >= 0) && (CurrentRow < 8))
	{
		if (StandardInitPosition->at(CurrentCol+CurrentRow*8) == '1')
		{
			AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
			CandidatesIndex++;
			CurrentCol--;
			CurrentRow++;
		}
		else
		{
			if (isWhite ? (isBlackToken(CurrentCol, CurrentRow)) : (isWhiteToken(CurrentCol, CurrentRow)))
			{
				AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
				CandidatesIndex++;
				break;
			}
			break;
		}
	}
	CurrentCol = Col + 1;
	CurrentRow = Row - 1;
	while ((CurrentCol < 8) && (CurrentRow >= 0))
	{
		if (StandardInitPosition->at(CurrentCol+CurrentRow*8) == '1')
		{
			AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
			CandidatesIndex++;
			CurrentCol++;
			CurrentRow--;
		}
		else
		{
			if (isWhite ? (isBlackToken(CurrentCol, CurrentRow)) : (isWhiteToken(CurrentCol, CurrentRow)))
			{
				AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
				CandidatesIndex++;
				break;
			}
			break;
		}
	}
}

void KingCandidatePos(int AvailablePos[], int Col, int Row, bool isWhite)
{
	int CandidatesIndex = 0;
	int CurrentCol = Col;
	int CurrentRow = Row;
	bool CheckOnPosition;
	for (int i_row = -1; i_row < 2; i_row++)
		for (int i_col = -1; i_col < 2; i_col++)
		{
			CheckOnPosition = false;
			if (!((i_row == 0) && (i_col == 0)))
			{
				CurrentCol = Col + i_row;
				CurrentRow = Row + i_col;
				if (((CurrentCol >= 0) && (CurrentCol < 8) && (CurrentRow >= 0) && (CurrentRow < 8)))
				{
					if (StandardInitPosition->at(CurrentCol + CurrentRow * 8) == '1')
					{
						AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
						CandidatesIndex++;
					}

					else
					{
						if (isWhite ? (isBlackToken(CurrentCol, CurrentRow)) : (isWhiteToken(CurrentCol, CurrentRow)))
						{
							AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
							CandidatesIndex++;
						}
					}
				}
			}
		}
}

void KnightCandidatePos(int AvailablePos[], int Col, int Row, bool isWhite)
{
	int CandidatesIndex = 0;
	int CurrentCol;
	int CurrentRow;

	//horizontal
	for (int i_col = -1; i_col < 2; i_col++)
		for (int i_row = -1; i_row < 2; i_row++)
		{
			CurrentCol = Col + i_col * 2;
			CurrentRow = Row + i_row;
			if ((CurrentCol >= 0) && (CurrentCol < 8) && (CurrentRow >= 0) && (CurrentRow < 8) && (i_row != 0) && (i_col != 0))
				if ((StandardInitPosition->at(CurrentCol+CurrentRow*8) == '1'))
				{
					AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
					CandidatesIndex++;
				}
				else
				{
					if (isWhite && (isBlackToken(CurrentCol, CurrentRow)))
					{
						AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
						CandidatesIndex++;
					}
					if (!isWhite && (isWhiteToken(CurrentCol, CurrentRow)))
					{
						AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
						CandidatesIndex++;
					}
				}
		}

	//vertical
	for (int i_row = -1; i_row < 2; i_row++)
		for (int i_col = -1; i_col < 2; i_col++)
		{
			CurrentCol = Col + i_col;
			CurrentRow = Row + i_row * 2;
			if ((CurrentRow >= 0) && (CurrentRow < 8) && (CurrentCol >= 0) && (CurrentCol < 8) && (i_row != 0) && (i_col != 0))
				if (StandardInitPosition->at(CurrentCol+CurrentRow*8) == '1')
				{
					AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
					CandidatesIndex++;
				}
				else
				{
					if (isWhite && (isBlackToken(CurrentCol, CurrentRow)))
					{
						AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
						CandidatesIndex++;
					}
					if (!isWhite && (isWhiteToken(CurrentCol, CurrentRow)))
					{
						AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
						CandidatesIndex++;
					}
				}
		}
}

void PawnCandidatePos(int AvailablePos[], int Col, int Row, bool isWhite, bool isPawnMovement)
{
	int CandidatesIndex = 0;
	int CurrentCol;
	int CurrentRow;

	if (isWhite)
	{
		CurrentCol = Col;
		CurrentRow = Row - 1;
		if (CurrentRow >= 0) 
		{
			if (isPawnMovement)
			{
				if (StandardInitPosition->at(CurrentCol + CurrentRow * 8) == '1')
				{
					AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
					CandidatesIndex++;
					CurrentRow = Row - 2;
					//si esta vacio el espacio enfrente y el 2do y esta en la posicion inicial
					if ((Row == 6) && (StandardInitPosition->at(CurrentCol + CurrentRow * 8) == '1'))
					{
						AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
						CandidatesIndex++;
					}
				}
				//si hay un peon enemigo a la derecha y el espacio que sigue en esa columna esta vacio
				if (((StandardInitPosition->at(CurrentCol + 1 + (Row - 1) * 8) == '1') &&
					((StandardInitPosition->at(CurrentCol + 1 + (Row) * 8) == 'p'))) || isBlackToken(CurrentCol + 1, Row - 1))
				{
					AvailablePos[CandidatesIndex] = Col + 1 + (Row - 1) * 8;
					CandidatesIndex++;
				}
				//si hay un peon enemigo a la izq y el espacio que sigue en esa columna esta vacio
				if (((StandardInitPosition->at(CurrentCol - 1 + (Row - 1) * 8) == '1') &&
					((StandardInitPosition->at(CurrentCol - 1 + (Row) * 8) == 'p'))) || isBlackToken(CurrentCol - 1, Row - 1))
				{
					AvailablePos[CandidatesIndex] = Col - 1 + (Row - 1) * 8;
					CandidatesIndex++;
				}
			}
			else
			{
				if (StandardInitPosition->at(CurrentCol + 1 + (Row - 1) * 8) == '1')
				{
					AvailablePos[CandidatesIndex] = Col + 1 + (Row - 1) * 8;
					CandidatesIndex++;
				}
				if (StandardInitPosition->at(CurrentCol - 1 + (Row - 1) * 8) == '1')
				{
					AvailablePos[CandidatesIndex] = Col - 1 + (Row - 1) * 8;
					CandidatesIndex++;
				}
			}
		}		
	}
	else
	{
		CurrentCol = Col;
		CurrentRow = Row + 1;
		if (CurrentRow < 8)
		{
			if (isPawnMovement)
			{
				if (StandardInitPosition->at(CurrentCol + CurrentRow * 8) == '1')
				{
					AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
					CandidatesIndex++;
					CurrentRow = Row + 2;
					//si esta vacio el espacio enfrente y el 2do y esta en la posicion inicial
					if ((Row == 1) && (StandardInitPosition->at(CurrentCol + CurrentRow * 8) == '1'))
					{
						AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
						CandidatesIndex++;
					}
				}
				//si hay un peon enemigo a la derecha y el espacio que sigue en esa columna esta vacio
				if (((StandardInitPosition->at(CurrentCol + 1 + (Row + 1) * 8) == '1') &&
					((StandardInitPosition->at(CurrentCol + 1 + (Row) * 8) == 'P'))) || isWhiteToken(CurrentCol + 1, Row + 1))
				{
					AvailablePos[CandidatesIndex] = Col + 1 + (Row + 1) * 8;
					CandidatesIndex++;
				}
				//si hay un peon enemigo a la izq y el espacio que sigue en esa columna esta vacio
				if (((StandardInitPosition->at(CurrentCol - 1 + (Row + 1) * 8) == '1') &&
					((StandardInitPosition->at(CurrentCol - 1 + (Row) * 8) == 'P'))) || isWhiteToken(CurrentCol - 1, Row + 1))
				{
					AvailablePos[CandidatesIndex] = Col - 1 + (Row + 1) * 8;
					CandidatesIndex++;
				}
			}
			else
			{
				if (StandardInitPosition->at(CurrentCol + 1 + (Row + 1) * 8) == '1')
				{
					AvailablePos[CandidatesIndex] = Col + 1 + (Row + 1) * 8;
					CandidatesIndex++;
				}
				if (StandardInitPosition->at(CurrentCol - 1 + (Row + 1) * 8) == '1')
				{
					AvailablePos[CandidatesIndex] = Col - 1 + (Row + 1) * 8;
					CandidatesIndex++;
				}
			}
		}
	}
}

void QueenCandidatePos(int AvailablePos[], int Col, int Row, bool isWhite)
{
	int CandidatesIndex = 0;
	int CurrentCol;
	int CurrentRow;

	CurrentCol = Col + 1;
	CurrentRow = Row + 1;
	while ((CurrentCol < 8) && (CurrentRow < 8))
	{
		//si esta vacio, se agrega, si no, se deja de buscar
		if (StandardInitPosition->at(CurrentCol+CurrentRow*8) == '1')
		{
			AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
			CandidatesIndex++;
			CurrentCol++;
			CurrentRow++;
		}
		else
		{
			if (isWhite ? (isBlackToken(CurrentCol, CurrentRow)) : (isWhiteToken(CurrentCol, CurrentRow)))
			{
				AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
				CandidatesIndex++;
				break;
			}
			break;
		}
	}
	CurrentCol = Col - 1;
	CurrentRow = Row - 1;
	while ((CurrentCol >= 0) && (CurrentRow >= 0))
	{
		if (StandardInitPosition->at(CurrentCol+CurrentRow*8) == '1')
		{
			AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
			CandidatesIndex++;
			CurrentCol--;
			CurrentRow--;
		}
		else
		{
			if (isWhite ? (isBlackToken(CurrentCol, CurrentRow)) : (isWhiteToken(CurrentCol, CurrentRow)))
			{
				AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
				CandidatesIndex++;
				break;
			}
			break;
		}
	}

	CurrentCol = Col - 1;
	CurrentRow = Row + 1;
	while ((CurrentCol >= 0) && (CurrentRow < 8))
	{
		if (StandardInitPosition->at(CurrentCol+CurrentRow*8) == '1')
		{
			AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
			CandidatesIndex++;
			CurrentCol--;
			CurrentRow++;
		}
		else
		{
			if (isWhite ? (isBlackToken(CurrentCol, CurrentRow)) : (isWhiteToken(CurrentCol, CurrentRow)))
			{
				AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
				CandidatesIndex++;
				break;
			}
			break;
		}
	}
	CurrentCol = Col + 1;
	CurrentRow = Row - 1;
	while ((CurrentCol < 8) && (CurrentRow >= 0))
	{
		if (StandardInitPosition->at(CurrentCol+CurrentRow*8) == '1')
		{
			AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
			CandidatesIndex++;
			CurrentCol++;
			CurrentRow--;
		}
		else
		{
			if (isWhite ? (isBlackToken(CurrentCol, CurrentRow)) : (isWhiteToken(CurrentCol, CurrentRow)))
			{
				AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
				CandidatesIndex++;
				break;
			}
			break;
		}
	}
	CurrentCol = Col;
	CurrentRow = Row + 1;
	//Up
	while ((CurrentCol < 8) && (CurrentRow < 8))
	{
		//si esta vacio, se agrega, si no, se deja de buscar
		if ((StandardInitPosition->at(CurrentCol+CurrentRow*8) == '1'))
		{
			AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
			CandidatesIndex++;
			CurrentRow++;
		}
		else
		{
			if (isWhite && isBlackToken(CurrentCol, CurrentRow))
			{
				AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
				CandidatesIndex++;
				break;
			}
			if (!isWhite && isWhiteToken(CurrentCol, CurrentRow))
			{
				AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
				CandidatesIndex++;
				break;
			}
			break;
		}
	}
	CurrentCol = Col;
	CurrentRow = Row - 1;
	//Down
	while ((CurrentCol >= 0) && (CurrentRow >= 0))
	{
		if ((StandardInitPosition->at(CurrentCol+CurrentRow*8) == '1'))
		{
			AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
			CandidatesIndex++;
			CurrentRow--;
		}
		else
		{
			if (isWhite && isBlackToken(CurrentCol, CurrentRow))
			{
				AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
				CandidatesIndex++;
				break;
			}
			if (!isWhite && isWhiteToken(CurrentCol, CurrentRow))
			{
				AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
				CandidatesIndex++;
				break;
			}
			break;
		}
	}

	CurrentCol = Col - 1;
	CurrentRow = Row;
	//left
	while ((CurrentCol >= 0) && (CurrentRow < 8))
	{
		if ((StandardInitPosition->at(CurrentCol+CurrentRow*8) == '1'))
		{
			AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
			CandidatesIndex++;
			CurrentCol--;
		}
		else
		{
			if (isWhite && isBlackToken(CurrentCol, CurrentRow))
			{
				AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
				CandidatesIndex++;
				break;
			}
			if (!isWhite && isWhiteToken(CurrentCol, CurrentRow))
			{
				AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
				CandidatesIndex++;
				break;
			}
			break;
		}
	}
	CurrentCol = Col + 1;
	CurrentRow = Row;
	//right
	while ((CurrentCol < 8) && (CurrentRow >= 0))
	{
		if ((StandardInitPosition->at(CurrentCol+CurrentRow*8) == '1'))
		{
			AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
			CandidatesIndex++;
			CurrentCol++;
		}
		else
		{
			if (isWhite && isBlackToken(CurrentCol, CurrentRow))
			{
				AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
				CandidatesIndex++;
				break;
			}
			if (!isWhite && isWhiteToken(CurrentCol, CurrentRow))
			{
				AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
				CandidatesIndex++;
				break;
			}
			break;
		}
	}

}

void RookCandidatePos(int AvailablePos[], int Col, int Row, bool isWhite)
{
	int CandidatesIndex = 0;
	int CurrentCol;
	int CurrentRow;


	CurrentCol = Col;
	CurrentRow = Row + 1;
	//Up
	while ((CurrentCol < 8) && (CurrentRow < 8))
	{
		//si esta vacio, se agrega, si no, se deja de buscar
		if ((StandardInitPosition->at(CurrentCol+CurrentRow*8) == '1'))
		{
			AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow*8;
			CandidatesIndex++;
			CurrentRow++;
		}
		else
		{
			if (isWhite && isBlackToken(CurrentCol, CurrentRow))
				{
					AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
					CandidatesIndex++;
					break;
				}
			if (!isWhite && isWhiteToken(CurrentCol, CurrentRow))
				{
					AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
					CandidatesIndex++;
					break;
				}
			break;
		}
	}
	CurrentCol = Col;
	CurrentRow = Row - 1;
	//Down
	while ((CurrentCol >= 0) && (CurrentRow >= 0))
	{
		if ((StandardInitPosition->at(CurrentCol+CurrentRow*8) == '1'))
		{
			AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
			CandidatesIndex++;
			CurrentRow--;
		}
		else
		{
			if (isWhite && isBlackToken(CurrentCol, CurrentRow))
			{
				AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
				CandidatesIndex++;
				break;
			}
			if (!isWhite && isWhiteToken(CurrentCol, CurrentRow))
			{
				AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
				CandidatesIndex++;
				break;
			}
			break;
		}
	}

	CurrentCol = Col - 1;
	CurrentRow = Row;
	//left
	while ((CurrentCol >= 0) && (CurrentRow < 8))
	{
		if ((StandardInitPosition->at(CurrentCol+CurrentRow*8) == '1'))
		{
			AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
			CandidatesIndex++;
			CurrentCol--;
		}
		else
		{
			if (isWhite && isBlackToken(CurrentCol, CurrentRow))
			{
				AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
				CandidatesIndex++;
				break;
			}
			if (!isWhite && isWhiteToken(CurrentCol, CurrentRow))
			{
				AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
				CandidatesIndex++;
				break;
			}
			break;
		}
	}
	CurrentCol = Col + 1;
	CurrentRow = Row;
	//right
	while ((CurrentCol < 8) && (CurrentRow >= 0))
	{
		if ((StandardInitPosition->at(CurrentCol+CurrentRow*8) == '1'))
		{
			AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
			CandidatesIndex++;
			CurrentCol++;
		}
		else
		{
			if (isWhite && isBlackToken(CurrentCol, CurrentRow))
			{
				AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
				CandidatesIndex++;
				break;
			}
			if (!isWhite && isWhiteToken(CurrentCol, CurrentRow))
			{
				AvailablePos[CandidatesIndex] = CurrentCol + CurrentRow * 8;
				CandidatesIndex++;
				break;
			}
			break;
		}
	}
}

bool isCandidateContained(int AvailablePos[], int Col, int Row)
{
	for (int i = 0; i < 28; i++)
	{
		if (AvailablePos[i] == Col+Row*8)
		{
			return true;
		}
	}
	return false;
}

bool isCastlingValid(char SelectedToken, int ToMoveCol, int ToMoveRow)
{
	//reviso horizontalmente si hay alguien que bloquee el camino
	if ((SelectedToken == 'K') && !w_KingMoved && (ToMoveRow==7))
	{
		if ((ToMoveCol == 6) && !w_7RookMoved)
		{
			//reviso hacia la derecha
			for (int i = (4 + 7 * 8) + 1; i < (4 + 2 + 7 * 8) + 1; i++)
			{
				if (StandardInitPosition->at(i) != '1') { return false; }
			}
			w_7RookMoved = true;
			w_KingMoved = true;
			doCastling(ToMoveCol, true);
			return true;
		}
		if ((ToMoveCol == 2) && !w_0RookMoved)
		{			
			//reviso hacia la izquierda
			for (int i = (4 + 7 * 8) - 1; i > (4 - 2 + 7 * 8) - 1; i--)
			{
				if (StandardInitPosition->at(i) != '1') { return false; }
			}
			w_0RookMoved = true;
			w_KingMoved = true;
			doCastling(ToMoveCol, true);
			return true;
		}
	}
	if ((SelectedToken == 'k') && !b_KingMoved && (ToMoveRow==0))
	{
		if ((ToMoveCol == 6) && !b_7RookMoved)
		{
			//reviso hacia la derecha
			for (int i = 4 + 1; i < (4 + 2) + 1; i++)
			{
				if (StandardInitPosition->at(i) != '1') { return false; }
			}
			b_7RookMoved = true;
			b_KingMoved = true;
			doCastling(ToMoveCol, false);
			return true;
		}
		if ((ToMoveCol == 2) && !b_0RookMoved)
		{
			//reviso hacia la izquierda
			for (int i = 4 - 1; i > (4 - 2) - 1; i--)
			{
				if (StandardInitPosition->at(i) != '1') { return false; }
			}
			b_0RookMoved = true;
			b_KingMoved = true;
			doCastling(ToMoveCol, false);
			return true;
		}
	}	
	return false;
}

void doCastling(int KingToMoveCol, bool isWhite)
{
	if (isWhite ? (RemoveToken(4, 7)) : (RemoveToken(4, 7)));
	
	if (KingToMoveCol > 4)
	{
		if (isWhite)
		{
			RemoveToken(7, 7);
			AddToken('K', 6, 7);
			AddToken('R', 5, 7);
		}
		else
		{
			RemoveToken(7, 0);
			AddToken('k', 6, 0);
			AddToken('r', 5, 0);
		}
	}
	else
	{
		if (isWhite)
		{
			RemoveToken(0, 7);
			AddToken('K', 2, 7);
			AddToken('R', 3, 7);
		}
		else
		{
			RemoveToken(0, 0);
			AddToken('k', 2, 0);
			AddToken('r', 3, 0);
		}
	}
}

bool isWhiteToken(int Col, int Row)
{
	if (((int)StandardInitPosition->at(Col + Row * 8) > 64) && ((int)StandardInitPosition->at(Col + Row * 8) < 91)) { return true; }
	return false;
}

bool isBlackToken(int Col, int Row)
{
	if (((int)StandardInitPosition->at(Col + Row * 8) > 96) && ((int)StandardInitPosition->at(Col + Row * 8) < 123)) { return true; }
	return false;
}

void ShowAvailablePositions(char Token, int Col, int Row)
{
	int* AvailablePos = new int[28];
	CalculateAvailablePos(AvailablePos, Token, Col, Row, true);
	for (int i = 0; i < 28; i++)
	{
		if (AvailablePos[i] != -1)
		{
			if (((Token == 'K') && (!(isThereCheckAt(AvailablePos[i], true, false)))) || ((Token == 'k') && (!(isThereCheckAt(AvailablePos[i], false, false)))) || (!((Token == 'k') || (Token == 'K'))))
			{
				SetSurfacePosition(AvailablePos[i] % 8 * TokenXSize, AvailablePos[i] / 8 * TokenYSize, gAvailable, gScreenSurface);
			}
		}
	}
}

bool isThereCheckAt(int KingIndex, bool isWhite, bool isPawnMovement)
{
	int* AvailablePos = new int[28];

	for (int i = 0; i < 64; i++)
	{
		if ((StandardInitPosition->at(i) != '1') && (i != KingIndex))
		{
			CalculateAvailablePos(AvailablePos, StandardInitPosition->at(i), i % 8, i / 8, isPawnMovement);
			for (int j = 0; j < 28; j++)
			{
				if (isWhite ? isBlackToken(i % 8, i / 8) && (AvailablePos[j] == KingIndex) && (AvailablePos[j] != -1) : isWhiteToken(i % 8, i / 8) && (AvailablePos[j] == KingIndex) && (AvailablePos[j] != -1))
				{
					delete[] AvailablePos;
					return true;
				}
			}
		}
	}
	delete[] AvailablePos;
	return false;
}

int FindKing(bool isWhite)
{
	for (int i = 0; i < 64; i++)
	{
		if ((((StandardInitPosition->at(i)) == 'K') && isWhite) || (((StandardInitPosition->at(i)) == 'k') && !isWhite)) { return i; }
	}
}

void CalculateAvailablePos(int AvailablePos[], char Token, int Col, int Row, bool isPawnMovement)
{
	//se usara el valor -1 para saber que elemento no cuenta
	fill_n(AvailablePos, 28, -1);
	//si el token esta en mayusculas, lo estandarizamos a minusculas para evitar multiples valores
	//es importante diferenciar los peones ya que se mueven en un solo sentido

	switch ((int)Token)
	{
		//bishop
	case 66:
		BishopCandidatePos(AvailablePos, Col, Row, true);
		break;
		//king
	case 75:
		KingCandidatePos(AvailablePos, Col, Row, true);
		break;
		//knight
	case 78:
		KnightCandidatePos(AvailablePos, Col, Row, true);
		break;
		//pawn
	case 80:
		PawnCandidatePos(AvailablePos, Col, Row, true, isPawnMovement);
		break;
		//queen
	case 81:
		QueenCandidatePos(AvailablePos, Col, Row, true);
		break;
		//rook
	case 82:
		RookCandidatePos(AvailablePos, Col, Row, true);
		break;
	case 98:
		BishopCandidatePos(AvailablePos, Col, Row, false);
		break;
		//king
	case 107:
		KingCandidatePos(AvailablePos, Col, Row, false);
		break;
		//knight
	case 110:
		KnightCandidatePos(AvailablePos, Col, Row, false);
		break;
		//pawn
	case 112:
		PawnCandidatePos(AvailablePos, Col, Row, false, isPawnMovement);
		break;
		//queen
	case 113:
		QueenCandidatePos(AvailablePos, Col, Row, false);
		break;
		//rook
	case 114:
		RookCandidatePos(AvailablePos, Col, Row, false);
		break;
	}
}