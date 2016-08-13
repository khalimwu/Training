// 6TicTacToe.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int CurrentCombination[9];
int LastValidCombination;
int Int_Combination = 123456789;
int Counter = 0;
char NewLine[26];


int main()
{
	fstream NewFile("TicTacToe.txt", ios::out);
	cout << "Generando los resultados en el archivo TicTacToe.txt. Esto va a demorar unos minutos. No cierre el programa. \n";
	listAllTicTacToeGames(NewFile);
	NewFile.close();
	cout << "Resultados generados. Se puede cerrar el programa. \n";
	system("PAUSE");
	return 0;
}

void listAllTicTacToeGames(fstream &TicTacToeFile)
{
	int WinningIndex;
	TicTacToeFile << "  N    X 0 X 0 X 0 X 0 X Win";
	while (Int_Combination != 999999999)
	{
		MakeCombination(CurrentCombination, Int_Combination);
		if (isValidCombination(CurrentCombination))
		{
			WinningIndex = GetWinner(CurrentCombination);
			if (LastValidCombination != ReformatCombination(CurrentCombination, abs(WinningIndex)))
			{
				if (WinningIndex != 0)
				{
					LastValidCombination = ReformatCombination(CurrentCombination, abs(WinningIndex));
				}
				else
				{
					LastValidCombination = Int_Combination;
				}

				if (WinningIndex > 0)
				{
					TicTacToeFile << "\n";
					Counter++;
					FormatLine(Counter, LastValidCombination, 'X', NewLine);
					TicTacToeFile << NewLine;
				}
				else if (WinningIndex < 0)
				{
					TicTacToeFile << "\n";
					Counter++;
					FormatLine(Counter, LastValidCombination, 'O', NewLine);
					TicTacToeFile << NewLine;
				}
				else
				{
					if ((isValidTie(CurrentCombination)))
					{
						TicTacToeFile << "\n";
						Counter++;
						FormatLine(Counter, LastValidCombination, 'E', NewLine);
						TicTacToeFile << NewLine;
					}
				}
			}

		}
		Int_Combination++;
	}
}

void FormatLine(int Counter, int Combination, char Winner, char NewLine[26])
{
	int NewLineIndex = 0;
	int c_CounterIndex = 5;

	while (Counter > 0)
	{
		NewLine[c_CounterIndex] = (Counter % 10)+48;
		Counter = Counter / 10;
		c_CounterIndex--;
	}
	for (int i = 0; i < c_CounterIndex + 1; i++)
	{
		NewLine[i] = '0';
	}
	
	NewLine[6] = ' ';
	NewLineIndex = 23;

	while (Combination > 0)
	{
		NewLine[NewLineIndex] = (Combination % 10) + 48;
		Combination = Combination / 10;
		NewLineIndex--;
		NewLine[NewLineIndex] = ' ';
		NewLineIndex--;
	}

	NewLine[24] = ' ';
	NewLine[25] = Winner;
}

int ReformatCombination(int Combination[9], int WinningTokenIndex)
{
	int ReformattedInt = 0;
	if (WinningTokenIndex != 0)
	{		//lleno los numeros hasta el WinningToken
		for (int i = 0; i < WinningTokenIndex; i++)
		{
			ReformattedInt = ReformattedInt * 10 + Combination[i];
		}
		//completo con 0s el resto de los tokens que no necesito
		for (int i = WinningTokenIndex; i < 9; i++)
		{
			ReformattedInt = ReformattedInt * 10;
		}
	}
	else
	{
		for (int i = 0; i < 9; i++)
		{
			ReformattedInt = ReformattedInt * 10 + Combination[i];
		}
	}

	return ReformattedInt;
}

void MakeCombination(int MakeCombination[9],int Int_Combination)
{
	for (int i = 0; i < 9; i++)
	{
		MakeCombination[8 - i] = Int_Combination % 10;
		Int_Combination = Int_Combination / 10;
	}
}

int GetWinner(int Combination[9])
{
	bool isPlayerX= true;
	int CurrentToken;
	int WinningCandidates[9];
	fill_n(WinningCandidates, 9, 10);
	int CurrentCandidateIndex = 0;
	//si se devuelve el index+1 = ganador X
	//si se devuelve el -(index+1)  = ganador O
	// 0 = Empate

	//jugadas ganadoras 
	//1->2->3, 4->5->6, 7->8->9
	//1->4->7, 2->5->8, 3->6->9
	//1->5->9, 3->5->7
	//si el jugador contrario tiene una ficha antes de la ultima jugada que ocasiona ganar esa combinacion, no es ganadora

	//los ganadores se veran apartir de la jugada 5

	//si N5 es 0, es porque no existe ganador

	for (int i = 0; i < 9; i++)
	{
		CurrentToken = Combination[i];
		//si la posicion en el array es par, estoy en el jugador X
		switch (CurrentToken)
		{
			//1->2->3
			//1->4->7
			//1->5->9
			// 1,2,4,5,6,8,0,0,0
		case 1:
			for (int j = 0; j < 9; j++)
			{
				CurrentToken = Combination[j];
				if ((CurrentToken == 2) || (CurrentToken == 3)
					|| ((CurrentToken == 4)) || (CurrentToken == 7)
					|| ((CurrentToken == 5)) || (CurrentToken == 9))
				{
					//si el otro jugador posee estas casillas, el jugador X no tiene la combinacion
					if (((isPlayerX) && (j % 2 == 0)) || ((!isPlayerX) && (j % 2 != 0)))
					{
						for (int k = j; k < 9; k++)
						{
							if ((((CurrentToken == 2) && (Combination[k] == 3)) || ((CurrentToken == 3) && (Combination[k] == 2))
								|| ((CurrentToken == 4) && (Combination[k] == 7)) || ((CurrentToken == 7) && (Combination[k] == 4))
								|| ((CurrentToken == 5) && (Combination[k] == 9)) || ((CurrentToken == 9) && (Combination[k] == 5)))
								&& (Combination[k] != CurrentToken))
							{
								if ((isPlayerX) && (k % 2 == 0))
								{
									if ((i > j) && (i > k))
									{
										WinningCandidates[CurrentCandidateIndex] = i + 1;
									}
									if ((j > i) && (j > k))
									{
										WinningCandidates[CurrentCandidateIndex] = j + 1;
									}
									if ((k > i) && (k > j))
									{
										WinningCandidates[CurrentCandidateIndex] = k + 1;
									}
									CurrentCandidateIndex++;
								}
								if ((!isPlayerX) && (k % 2 != 0))
								{
									if ((i > j) && (i > k))
									{
										WinningCandidates[CurrentCandidateIndex] = (-1)*(i + 1);
									}
									if ((j > i) && (j > k))
									{
										WinningCandidates[CurrentCandidateIndex] = (-1)*(j + 1);
									}
									if ((k > i) && (k > j))
									{
										WinningCandidates[CurrentCandidateIndex] = (-1)*(k + 1);
									}
									CurrentCandidateIndex++;
								}
							}
						}
					}
				}
			}
			break;
			//4->5->6
			//2->5->8
			//3->5->7
			//  5   9   6   3   7
			//    4   8   1   2
		case 5:
			for (int j = 0; j < 9; j++)
			{
				CurrentToken = Combination[j];
				if ((CurrentToken == 2) || (CurrentToken == 3)
					|| ((CurrentToken == 4)) || (CurrentToken == 6)
					|| ((CurrentToken == 7)) || (CurrentToken == 8))
				{
					//si el otro jugador posee estas casillas, el jugador X no tiene la combinacion
					if (((isPlayerX) && (j % 2 == 0)) || ((!isPlayerX) && (j % 2 != 0)))
					{
						for (int k = j; k < 9; k++)
						{
							if ((((CurrentToken == 4) && (Combination[k] == 6)) || ((CurrentToken == 6) && (Combination[k] == 4))
								|| ((CurrentToken == 2) && (Combination[k] == 8)) || ((CurrentToken == 8) && (Combination[k] == 2))
								|| ((CurrentToken == 3) && (Combination[k] == 7)) || ((CurrentToken == 7) && (Combination[k] == 3)))
								&& (Combination[k] != CurrentToken))
							{
								if ((isPlayerX) && (k % 2 == 0))
								{
									if ((i > j) && (i > k))
									{
										WinningCandidates[CurrentCandidateIndex] = i + 1;
									}
									if ((j > i) && (j > k))
									{
										WinningCandidates[CurrentCandidateIndex] = j + 1;
									}
									if ((k > i) && (k > j))
									{
										WinningCandidates[CurrentCandidateIndex] = k + 1;
									}
									CurrentCandidateIndex++;
								}
								if ((!isPlayerX) && (k % 2 != 0))
								{
									if ((i > j) && (i > k))
									{
										WinningCandidates[CurrentCandidateIndex] = (-1)*(i + 1);
									}
									if ((j > i) && (j > k))
									{
										WinningCandidates[CurrentCandidateIndex] = (-1)*(j + 1);
									}
									if ((k > i) && (k > j))
									{
										WinningCandidates[CurrentCandidateIndex] = (-1)*(k + 1);
									}
									CurrentCandidateIndex++;
								}
							}
						}
					}
				}
			}
			break;
			//7->8->9
			//3->6->9
		case 9:
			for (int j = 0; j < 9; j++)
			{
				CurrentToken = Combination[j];
				if ((CurrentToken == 7) || (CurrentToken == 8)
					|| ((CurrentToken == 3)) || (CurrentToken == 6))
				{
					//si el otro jugador posee estas casillas, el jugador X no tiene la combinacion
					if (((isPlayerX) && (j % 2 == 0)) || ((!isPlayerX) && (j % 2 != 0)))
					{
						for (int k = j; k < 9; k++)
						{
							if ((((CurrentToken == 7) && (Combination[k] == 8)) || ((CurrentToken == 8) && (Combination[k] == 7))
								|| ((CurrentToken == 3) && (Combination[k] == 6)) || ((CurrentToken == 6) && (Combination[k] == 3)))
								&& (Combination[k] != CurrentToken))
							{
								if ((isPlayerX) && (k % 2 == 0)) 
								{ 
									if ((i > j) && (i > k))
									{
										WinningCandidates[CurrentCandidateIndex] = i + 1; 
									}
									if ((j > i) && (j > k))
									{
										WinningCandidates[CurrentCandidateIndex] = j + 1;
									}
									if ((k > i) && (k > j))
									{
										WinningCandidates[CurrentCandidateIndex] = k + 1;
									}									
									CurrentCandidateIndex++;
								}
								if ((!isPlayerX) && (k % 2 != 0))
								{
									if ((i > j) && (i > k))
									{
										WinningCandidates[CurrentCandidateIndex] = (-1)*(i + 1);
									}
									if ((j > i) && (j > k))
									{
										WinningCandidates[CurrentCandidateIndex] = (-1)*(j + 1);
									}
									if ((k > i) && (k > j))
									{
										WinningCandidates[CurrentCandidateIndex] = (-1)*(k + 1);
									}
									CurrentCandidateIndex++;
								}
							}
						}
					}
				}
			}
			break;
		}
		isPlayerX = !isPlayerX;
	}

	//obtengo el menor WinningIndex+1 de los candidatos
	int CurrentLowerIndex = 10;
	for (int i = 0; i < 9; i++)
	{
		if (abs(WinningCandidates[i]) < abs(CurrentLowerIndex))
		{
			CurrentLowerIndex = WinningCandidates[i];
		}
	}

	if (CurrentLowerIndex != 10) { return CurrentLowerIndex; }
	return 0;
}

bool isValidCombination(int Combination[9])
{
	for (int i = 0; i < 8; i++)
	{
		//si se encuentra un 0 y alguno de los que sigue no es 0, la combinacion es invalida
		if (Combination[i]==0)
			for (int j = i; j < 9; j++)
			{
				if (Combination[j] != 0)
				{
					return false;
				}
			}
		//si se repite un numero, la combinacion no es valida
		for (int j = i + 1; j < 9; j++)
		{
			if (Combination[j] == Combination[i])
			{
				return false;
			}
		}
	}


	return true;
}


bool isValidTie(int Combination[9])
{
	//Si se identifica un empate (la combinacion puede ser valida pero por espacios vacios puede haber un empate), volver a verificar que todas las casillas esten llenas
	//si se encuentra una casilla vacia y hubo un empate, no es una partida terminada
	for (int i = 0; i < 9; i++)
	{
		if (Combination[i] == 0) { return false; }
	}
	return true;
}