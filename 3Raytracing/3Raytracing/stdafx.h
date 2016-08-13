// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

class FVector
{
private:
	float X;
	float Y;
	float Z;

public:
	FVector()
	{
	}

	~FVector()
	{
	}

	FVector(float _X, float _Y, float _Z)
	{
		X = _X;
		Y = _Y;
		Z = _Z;
	}

	void SetCoords(float _X, float _Y, float _Z)
	{
		X = _X;
		Y = _Y;
		Z = _Z;
	}

	float GetX()
	{
		return X;
	}

	float GetY()
	{
		return Y;
	}

	float GetZ()
	{
		return Z;
	}

};

FVector getShadowPoint(FVector Q, FVector P1, FVector P2, FVector P3, FVector V);


// TODO: reference additional headers your program requires here
