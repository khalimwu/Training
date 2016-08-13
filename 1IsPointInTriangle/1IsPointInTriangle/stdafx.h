// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

struct FVector
{
	int X;
	int Y;

	void SetCoords(int _X, int _Y)
	{
		X = _X;
		Y = _Y;
	}
};

bool isPointOnLine(FVector P1, FVector P2, FVector ToCheck);

bool isPointInTriangle(FVector P1, FVector P2, FVector P3, FVector ToCheck);

// TODO: reference additional headers your program requires here
