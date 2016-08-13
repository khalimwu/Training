// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <iostream>

using namespace std;

void GenerateMaze(int nSize);

void DrawMaze();

void UpdateCells(int CurrentCell, int ToMoveCell);

bool isValidNeighbor(int CurrentCell, int ToMoveCell);

int PickRandomNeighbor(int CurrentCell);

int SizeOfBranch(int RootOfBranch, int Direction, int &LastIndex);

void GetNeighbors(int CurrentIndex, int LastIndex, int &N1, int &N2);

int GetNextNeighbor(int CurrentIndex, int LastIndex);

int EmptyNeighbors(int Index);

bool isBorder(int Index);

// TODO: reference additional headers your program requires here
