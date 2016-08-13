// 5Poker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <algorithm>
#include <vector>
#include <array>

char p1Hand[10];
char p2Hand[10];
string Cachedp1Hand;
string Cachedp2Hand;

int p1Score = 0;
int p2Score = 0;


int main()
{
	fstream* pokerFile = new fstream("poker.txt", fstream::in);
	
	//pokerFile.open("poker.txt");
	int currentIndex = 0;

	int p1wincount = 0;
	int p2wincount = 0;

	int p1HandCardValues[5];
	int p2HandCardValues[5];

	char c = ' ';

	while ((pokerFile->get(c)))
	{
		while (c != '\n')
		{
			if (isalpha(c) || isdigit(c))
			{
				if (currentIndex < 10)
				{
					p1Hand[currentIndex] = c;
				}
				else
				{
					p2Hand[currentIndex - 10] = c;
				}
				currentIndex++;
			}
			pokerFile->clear();
			c = pokerFile->get();
		}

		currentIndex = 0;
		c = ' ';
		
		SortHand(p1Hand);
		Cachedp1Hand = p1Hand;
		p1Score = GetHighestScoreInHand(p1Hand);
		SortHand(p2Hand);
		Cachedp2Hand = p2Hand;
		p2Score = GetHighestScoreInHand(p2Hand);
		GetCardValues(p1Hand, p1HandCardValues);
		GetCardValues(p2Hand, p2HandCardValues);

		if (p1Score > p2Score)
		{
			cout << Cachedp1Hand.c_str() << " " << Cachedp2Hand.c_str() << "  Ganador Player1" << "\n";
			p1wincount++;
		}
		else
		{
			///si el score es el mismo, se comparan las cartas
			if (p1Score == p2Score)
			{
				///si es fullhouse, se comparan primero los trios, si los trios son iguales, se comparan los pares, si los pares son iguales, se obtiene la carta mayor
				if (p1Score == 7)
				{
					if (GetHigherCardOfScore(p1HandCardValues, 4) > GetHigherCardOfScore(p2HandCardValues, 4))
					{
						cout << Cachedp1Hand.c_str() << " " << Cachedp2Hand.c_str() << "  Ganador Player1" << "\n";
						p1wincount++;
					}
					else if (GetHigherCardOfScore(p1HandCardValues, 4) == GetHigherCardOfScore(p2HandCardValues, 4))
					{
						if (GetHigherCardOfScore(p1HandCardValues, 2) > GetHigherCardOfScore(p2HandCardValues, 2))
						{
							cout << Cachedp1Hand.c_str() << " " << Cachedp2Hand.c_str() << "  Ganador Player1" << "\n";
							p1wincount++;
						}
						else if (GetHigherCardOfScore(p1HandCardValues, 4) == GetHigherCardOfScore(p2HandCardValues, 4))
						{
							if (GetHigherCardOfScore(p1HandCardValues, 1) > GetHigherCardOfScore(p2HandCardValues, 1))
							{
								cout << Cachedp1Hand.c_str() << " " << Cachedp2Hand.c_str() << "  Ganador Player1" << "\n";
								p1wincount++;
							}
						}
					}
				}
				else
				{
					if (GetHigherCardOfScore(p1HandCardValues, p1Score) > GetHigherCardOfScore(p2HandCardValues, p2Score))
					{
						cout << Cachedp1Hand.c_str() << " " << Cachedp2Hand.c_str() << "  Ganador Player1" << "\n";
						p1wincount++;
					}
					else
						//si las cartas en las combinaciones son iguales, se obtiene la carta de mayor tamanio
						if (GetHigherCardOfScore(p1HandCardValues, p1Score) == GetHigherCardOfScore(p2HandCardValues, p2Score))
						{
							if (GetHigherCardOfScore(p1HandCardValues, 1) > GetHigherCardOfScore(p2HandCardValues, 1))
							{
								cout << Cachedp1Hand.c_str() << " " << Cachedp2Hand.c_str() << "  Ganador Player1" << "\n";
								p1wincount++;
							}
						}
				}
			}
		}
	}

	cout << "Cantidad de manos ganadas por el jugador 1: " << p1wincount << "\n";

	system("PAUSE");
    return 0;
}

int GetHighestScoreInHand(char PlayerHand[])
{
	int HandValue;

	bool Pairs = false;
	bool TwoPairs = false;
	bool Three = false;
	bool Quad = false;

	///score va de 1 a 10

	HandValue = GetCardValue(PlayerHand[0]) + GetCardValue(PlayerHand[2]) + GetCardValue(PlayerHand[4]) + GetCardValue(PlayerHand[6]) + GetCardValue(PlayerHand[8]);

	DoCardCount(PlayerHand, Pairs, TwoPairs, Three, Quad);
	//cout << Pairs << "," << TwoPairs << "," << Three << "," << Quad << "\n";

	///si son todos del mismo tipo
	if (isSameType(PlayerHand))
	{
		//royal flush
		if (HandValue == 60) { return 10;}
		//flush
		if (isStraight(PlayerHand)) { return 9; }
		//si no es FullHouse ni fourOfAKind y sigue siendo del mismo tipo es flush
		if ((!Quad) && (!(Three && Pairs))) { return 6; }
	}
	else
	///si no son del mismo tipo
	{	
		//fourOfAKind
		if (Quad) { return 8; }
		//fullHouse
		if (Three && Pairs) { return 7; }
		//straight
		if (isStraight(PlayerHand)) { return 5; }
		//threeOfAKind
		if (Three) { return 4; }
		//twoPairs
		if (TwoPairs) { return 3; }
		//Pairs
		if (Pairs) { return 2; }
		//si obtiene 1, el mayor valor de HigherCard nos dara el ganador	
	}
	return 1;
}

int GetCardValue(char c)
{
	if (((int)c >= 50) && ((int)c <= 57))
	{
		return ((int)c - 48);
	}
	if ((c == 'T') || (c=='t')) { return 10; }
	if ((c == 'J') || (c == 'j')) { return 11; }
	if ((c == 'Q') || (c == 'q')) { return 12; }
	if ((c == 'K') || (c == 'k')) { return 13; }
	if ((c == 'A') || (c == 'a')) { return 14; }
	return 0;
}

///ordenamos la mano de menor a mayor
void SortHand(char PlayerHand[])
{
	char CachedNumber = ' ';
	char CachedType = ' ';
	for (unsigned i = 0; i < 5; i++)
	{
		for (unsigned j = i+1; j < 5; j++)
		{
			if (GetCardValue(PlayerHand[i * 2]) > GetCardValue(PlayerHand[j * 2]))
			{
				CachedType = PlayerHand[i * 2 + 1];
				CachedNumber = PlayerHand[i * 2];
				PlayerHand[i * 2] = PlayerHand[j * 2];
				PlayerHand[i * 2 + 1] = PlayerHand[j * 2 + 1];
				PlayerHand[j * 2 + 1] = CachedType;
				PlayerHand[j * 2] = CachedNumber;
			}
		}
	}
}

bool isStraight(char PlayerHand[])
{
	if ((GetCardValue(PlayerHand[8]) - GetCardValue(PlayerHand[6]) == 1)
		&& (GetCardValue(PlayerHand[6]) - GetCardValue(PlayerHand[4]) == 1)
			&& (GetCardValue(PlayerHand[4]) - GetCardValue(PlayerHand[2]) == 1)
				&& (GetCardValue(PlayerHand[2]) - GetCardValue(PlayerHand[0]) == 1))
	{
		return true;
	}
	return false;
}

void GetCardValues(char PlayerHand[], int CardValues[])
{
	int CurrentHigher = 0;
	int CurrentCardValuesIndex = 0;

	while (CurrentCardValuesIndex != 5)
	{
		CurrentHigher = GetCardValue(PlayerHand[0]);
		for (unsigned i = 1; i < 5; i++)
		{
			if ((GetCardValue(PlayerHand[i * 2]) > CurrentHigher))
			{
				CurrentHigher = GetCardValue(PlayerHand[i * 2]);
			}

		}

		for (unsigned k = 0; k < 5; k++)
		{
			if (GetCardValue(PlayerHand[k * 2]) == CurrentHigher)
			{
				CardValues[CurrentCardValuesIndex] = CurrentHigher;
				CurrentCardValuesIndex++;
				PlayerHand[k * 2] = ' ';
			}
		}
	}
	return;
}

void DoCardCount(char PlayerHand[], bool &Pairs, bool &TwoPairs, bool &Three, bool &Quad)
{
	int CurrentCardValue = 0;
	int CurrentCount = 1;

	for (unsigned i = 0; i < 5; i++)
	{
		CurrentCount = 1;
		for (unsigned j = 0; j < 5; j++)
		{
			if ((i != j) && (GetCardValue(PlayerHand[i * 2]) == GetCardValue(PlayerHand[j * 2])))
			{
				CurrentCount++;
				CurrentCardValue = GetCardValue(PlayerHand[i * 2]);
			}
		}

		if (CurrentCount == 4)
		{
			Quad = true;
			return;
		}

		if (CurrentCount == 3)
		{
			Three = true;
		}

		if (CurrentCount == 2)
		{
			Pairs = true;
		}
		if (Three || Pairs)
		{
			for (unsigned k = 0; k < 5; k++)
			{
				if ((GetCardValue(PlayerHand[k * 2]) != CurrentCardValue))
				{
					CurrentCount = 1;
					for (unsigned l = 0; l < 5; l++)
					{
						if ((k != l) && (GetCardValue(PlayerHand[k * 2]) == GetCardValue(PlayerHand[l * 2])))
						{
							CurrentCount++;
						}
					}

					if (CurrentCount == 3)
					{
						Three = true;
						return;
					}

					if (CurrentCount == 2)
					{
						if (Pairs)
						{
							TwoPairs = true;
						}
						if (Three)
						{
							Pairs = true;
						}
						return;
					}
				}
			}
		}
	}
	return;
}

bool isSameType(char PlayerHand[])
{
	return ((PlayerHand[1] == PlayerHand[3]) && (PlayerHand[3] == PlayerHand[5]) && (PlayerHand[5] == PlayerHand[7]) && (PlayerHand[7] == PlayerHand[9]));
}


int GetHigherCardOfScore(int CardValues[], int Score)
{
	int CurrentHigher = 0;
	int CurrentCount = 1;

	switch (Score)
	{
	case 9:
		return CardValues[0];
	case 8:
	{
		for (int i = 0; i < 4; i++)
		{
			if (CardValues[i] == CardValues[i + 1])
			{
				return CardValues[i];
			}
		}
	}
	//fullhouse se manejara como un trio y luego un par por separado
	case 7:
		break;
	case 6:
		return CardValues[0];
	case 5:
		return CardValues[0];
	case 4:
		for (int i = 0; i < 5; i++)
		{
			CurrentCount = 1;
			for (int j = 0; j < 5; j++)
			{
				if ((i != j) && (CardValues[i] == CardValues[j]))
				{
					CurrentCount++;
				}
			}
			if (CurrentCount == 3)
			{
				return CardValues[i];
			}
		}
	case 3:
		for (int i = 0; i < 4; i++)
		{
			if ((CardValues[i] == CardValues[i + 1]) && CardValues[i]>CurrentHigher)
			{
				CurrentHigher=CardValues[i+1];
				CardValues[i + 1] = 0;
			}
		}
		return CurrentHigher;
	case 2:
		for (int i = 0; i < 4; i++)
		{
			if (CardValues[i] == CardValues[i + 1])
			{
				return CardValues[i];
			}
		}
	case 1:
		return CardValues[0];
	}

}