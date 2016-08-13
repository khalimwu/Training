// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <fstream>
#include <iostream>

using namespace std;

int GetWinner(int Combination[9]);

bool isValidCombination(int Combination[9]);

void MakeCombination(int MakeCombination[9], int Int_Combination);

int ReformatCombination(int Combination[9], int WinningToken);

bool isValidTie(int Combination[9]);

void FormatLine(int Counter, int Combination, char Winner, char NewLine[26]);

void listAllTicTacToeGames(fstream &TicTacToeFile);

// TODO: reference additional headers your program requires here
