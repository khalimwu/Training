// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <fstream>

using namespace std;

int GetHighestScoreInHand(char PlayerHand[]);

int GetCardValue(char c);

void SortHand(char PlayerHand[]);

bool isStraight(char PlayerHand[]);

bool isSameType(char PlayerHand[]);

void GetCardValues(char PlayerHand[], int CardValues[]);

void DoCardCount(char PlayerHand[], bool &Pairs, bool &TwoPairs, bool &Three, bool &Quad);

int GetHigherCardOfScore(int CardValues[], int Score);

// TODO: reference additional headers your program requires here
