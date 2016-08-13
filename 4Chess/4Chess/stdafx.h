// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"
#include "SDL2-2.0.4\include\SDL.h"
#include "SDL2-2.0.4\include\SDL_image.h"

#include <stdio.h>
#include <tchar.h>
#include <string>
#include <iostream>
#include <cstdlib>

#define _CSURFACE_H_

using namespace std;


enum PlayerTokens
{
	TOKEN_PAWN,
	TOKEN_BISHOP,
	TOKEN_ROOK,
	TOKEN_KNIGHT,
	TOKEN_QUEEN,
	TOKEN_KING,
	SURFACE_TOTAL
};

bool init();

bool loadMedia();

void close();


SDL_Surface* loadSurface(string path);

void SetSurfacePosition(int x, int y, SDL_Surface* Surface, SDL_Surface* Destination);

void drawChessBoard();

bool isValidInput(string InitPosition);

void MakeStandardFormat(string InitPosition);

void GetBoardColRow(int InputX, int InputY, int &Col, int &Row);

char GetTokenChar(int Col, int Row);

char RemoveToken(int Col, int Row);

void AddToken(char Token, int Col, int Row);

bool isValidMove(char Token, int Col, int Row, int ToMoveCol, int ToMoveRow);

void BishopCandidatePos(int AvailablePos[], int Col, int Row, bool isWhite);

void KingCandidatePos(int AvailablePos[], int Col, int Row, bool isWhite);

void KnightCandidatePos(int AvailablePos[], int Col, int Row, bool isWhite);

void PawnCandidatePos(int AvailablePos[], int Col, int Row, bool isWhite, bool isPawnMovement);

void QueenCandidatePos(int AvailablePos[], int Col, int Row, bool isWhite);

void RookCandidatePos(int AvailablePos[], int Col, int Row, bool isWhite);

bool isCandidateContained(int AvailablePos[], int Col, int Row);

bool isCastlingValid(char SelectedToken, int ToMoveCol, int ToMoveRow);
							
void doCastling(int KingToMoveCol, bool isWhite);

bool isWhiteToken(int Col, int Row);

bool isBlackToken(int Col, int Row);

bool isThereCheckAt(int Index, bool isWhite, bool isPawnMovement);

int FindKing(bool isWhite);

void CalculateAvailablePos(int AvailablePos[], char Token, int Col, int Row, bool isPawnMovement);

void ShowAvailablePositions(char Token, int Col, int Row);
// TODO: reference additional headers your program requires here
