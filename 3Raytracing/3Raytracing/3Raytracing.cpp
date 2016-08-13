// 3Raytracing.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

using namespace std;

bool bNoShadow = false;

int main()
{
	FVector P1;
	FVector P2;
	FVector P3;
	FVector Q;
	FVector V;

	float Cin_X;
	float Cin_Y;
	float Cin_Z;

	char comma1 = ' ';
	char comma2 = ' ';
	
	while (1)
	{
		cout << "Introduzca el valor de P1 (Xf,Yf,Zf)" << "\n";
		cin >> Cin_X >> comma1 >> Cin_Y >> comma2 >> Cin_Z;
		if (cin.good())
		{
			P1.SetCoords(Cin_X, Cin_Y, Cin_Z);
			break;
		}
		else
		{
			cout << "Valores incorrectos, vuelva a intentar (Formato Xf,Yf,Zf)" << "\n";
			cin.clear();
			cin.ignore(INT_MAX, '\n');
		}
	}

	while (1)
	{
		cout << "Introduzca el valor de P2 (Xf,Yf,Zf)" << "\n";
		cin >> Cin_X >> comma1 >> Cin_Y >> comma2 >> Cin_Z;
		if (cin.good())
		{
			P2.SetCoords(Cin_X, Cin_Y, Cin_Z);
			break;
		}
		else
		{
			cout << "Valores incorrectos, vuelva a intentar (Formato Xf,Yf,Zf)" << "\n";
			cin.clear();
			cin.ignore(INT_MAX, '\n');
		}
	}

	while (1)
	{
		cout << "Introduzca el valor de P3 (Xf,Yf,Zf)" << "\n";
		cin >> Cin_X >> comma1 >> Cin_Y >> comma2 >> Cin_Z;
		if (cin.good())
		{
			P3.SetCoords(Cin_X, Cin_Y, Cin_Z);
			break;
		}
		else
		{
			cout << "Valores incorrectos, vuelva a intentar (Formato Xf,Yf,Zf)" << "\n";
			cin.clear();
			cin.ignore(INT_MAX, '\n');
		}
	}

	while (1)
	{
		cout << "Introduzca el valor de Q (Xf,Yf,Zf)" << "\n";
		cin >> Cin_X >> comma1 >> Cin_Y >> comma2 >> Cin_Z;
		if (cin.good())
		{
			Q.SetCoords(Cin_X, Cin_Y, Cin_Z);
			break;
		}
		else
		{
			cout << "Valores incorrectos, vuelva a intentar (Formato Xf,Yf,Zf)" << "\n";
			cin.clear();
			cin.ignore(INT_MAX, '\n');
		}
	}

	while (1)
	{
		cout << "Introduzca el valor de V (Xf,Yf,Zf)" << "\n";
		cin >> Cin_X >> comma1 >> Cin_Y >> comma2 >> Cin_Z;
		if (cin.good())
		{
			V.SetCoords(Cin_X, Cin_Y, Cin_Z);
			break;
		}
		else
		{
			cout << "Valores incorrectos, vuelva a intentar (Formato Xf,Yf,Zf)" << "\n";
			cin.clear();
			cin.ignore(INT_MAX, '\n');
		}
	}

	FVector ShadowPoint;
	ShadowPoint = getShadowPoint(Q, P1, P2, P3, V);

	if (bNoShadow)
	{
		cout << "No hay sombra" << "\n";
	}
	else
	{
		cout << ShadowPoint.GetX() << "," << ShadowPoint.GetY() << "," << ShadowPoint.GetZ() << "\n";
	}


	system("PAUSE");
    return 0;
}

FVector getShadowPoint(FVector Q, FVector P1, FVector P2, FVector P3, FVector V)
{
	FVector ResultPoint;
	
	//V line trace formula
	float VQ_X;
	float VQ_Y;
	float VQ_Z;
	float t = 0.0f;

	//VQ_X = Q.GetX() + V.GetX()*t;
	//VQ_Y = Q.GetY() + V.GetY()*t;
	//VQ_Z = Q.GetZ() + V.GetZ()*t;


	//Plane formula a(x-X0) - b(y-Y0) + c(Z-Z0) = 0
	float a;
	float b;
	float c;

	FVector V1(P1.GetX() - P2.GetX(), P1.GetY() - P2.GetY(), P1.GetZ() - P2.GetZ());
	FVector V2(P2.GetX() - P3.GetX(), P2.GetY() - P3.GetY(), P2.GetZ() - P3.GetZ());

	/// a           b         c
	///V1.GetX(),V1.GetY(),V1.GetZ()
	///V2.GetX(),V2.GetY(),V2.GetZ()

	a = V1.GetY()*V2.GetZ() - V1.GetZ()*V2.GetY();
	b = V1.GetX()*V2.GetZ() - V1.GetZ()*V2.GetX();
	c = V1.GetX()*V2.GetY() - V1.GetY()*V2.GetX();

	if ((a*(V.GetX()) - b*(V.GetY()) + c*(V.GetZ())) != 0)
	{
		t = -(a*(Q.GetX() - P1.GetX()) - b*(Q.GetY() - P1.GetY()) + c*(Q.GetZ() - P1.GetZ())) / (a*(V.GetX()) - b*(V.GetY()) + c*(V.GetZ()));
	}
	else
	{
		bNoShadow = true;
	}

	VQ_X = Q.GetX() + V.GetX()*t;
	VQ_Y = Q.GetY() + V.GetY()*t;
	VQ_Z = Q.GetZ() + V.GetZ()*t;

	ResultPoint.SetCoords(VQ_X, VQ_Y, VQ_Z);

	return ResultPoint;
}