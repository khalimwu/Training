// 7Laberinto.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <random>


bool* isBlock;
bool* isAlive;
bool* isVisited;
int* VisitedCellIndexes;
int LastVisitedCellIndex=0;
bool isBackTracking;
int SideSize;
int ExitIndex;

int main()
{	
	GenerateMaze(21);
	system("PAUSE");

    return 0;
}


void GenerateMaze(int nSize)
{
	/////Para la generacion del laberinto se considerara unicamente los segmentos internos. en la representacion final se agregara el borde.
	////Populate Grid
	SideSize = nSize - 2;
	isBlock = new bool[SideSize*SideSize];
	//true == ocupado
	fill_n(isBlock, SideSize*SideSize, true);
	VisitedCellIndexes = new int[SideSize*SideSize];
	fill_n(VisitedCellIndexes, SideSize*SideSize, -1);
	isAlive = new bool[SideSize*SideSize];
	fill_n(isAlive, SideSize*SideSize, true);
	isVisited = new bool[SideSize*SideSize];
	fill_n(isVisited, SideSize*SideSize, false);

	//se usara el algoritmo Growing Tree para generar el laberinto
	int CurrentCellIndex = 0;
	int ToMoveCell = 0;

	isBlock[0] = false;
	isAlive[CurrentCellIndex] = false;
	isVisited[CurrentCellIndex] = true;
	VisitedCellIndexes[LastVisitedCellIndex] = CurrentCellIndex;
	LastVisitedCellIndex++;

	while (LastVisitedCellIndex != 0)
	{
		ToMoveCell = PickRandomNeighbor(CurrentCellIndex);
		UpdateCells(CurrentCellIndex, ToMoveCell);
		if (isBackTracking)
		{
			CurrentCellIndex = VisitedCellIndexes[LastVisitedCellIndex];
		}
		else
		{
			CurrentCellIndex = ToMoveCell;
		}
	}
	
	int Direction = 1;

	if (isBlock[Direction]) { Direction = SideSize; };
	cout << "Path Length: " << SizeOfBranch(0, Direction, ExitIndex) << "\n";
	cout << "Exit Index: " << ExitIndex << "\n";

	DrawMaze();
}


void GetNeighbors(int CurrentIndex, int LastIndex, int &N1, int &N2)
{
	if ((!isBlock[CurrentIndex + SideSize]) && isValidNeighbor(CurrentIndex, CurrentIndex + SideSize) && ((CurrentIndex + SideSize)!= LastIndex))
	{
		N1 = CurrentIndex + SideSize;
	}
	if ((!isBlock[CurrentIndex - SideSize]) && isValidNeighbor(CurrentIndex, CurrentIndex - SideSize) && ((CurrentIndex - SideSize) != LastIndex))
	{
		if (N1 == -1)
		{
			N1 = CurrentIndex - SideSize;
		}
		else
		{
			N2 = CurrentIndex - SideSize;
			return;
		}
	}
	if ((!isBlock[CurrentIndex + 1]) && isValidNeighbor(CurrentIndex, CurrentIndex + 1) && ((CurrentIndex + 1) != LastIndex))
	{
		if (N1 == -1)
		{
			N1 = CurrentIndex + 1;
		}
		else
		{
			N2 = CurrentIndex + 1;
			return;
		}
	}
	if ((!isBlock[CurrentIndex - 1]) && isValidNeighbor(CurrentIndex, CurrentIndex - 1) && ((CurrentIndex - 1) != LastIndex))
	{
		if (N1 == -1)
		{
			N1 = CurrentIndex - 1;
		}
		else
		{
			N2 = CurrentIndex - 1;
			return;
		}
	}

}

int SizeOfBranch(int RootOfBranch, int Direction, int &ExitIndex)
{
	int BranchLength = 1;
	int N1 = -1;
	int N2 = -1;
	//direction tiene que estar un paso adelante del root
	int CurrentIndex = Direction;
	int LastIndex = RootOfBranch;
	int NextNeighbor;
	int N1LeafIndex = 0;
	int N2LeafIndex = 0;
	while (1)
	{
		//si es borde, tomar el current index como candidato a ser salida
		if (isBorder(CurrentIndex))
		{
			ExitIndex = CurrentIndex;
		}
		switch (EmptyNeighbors(CurrentIndex))
		{
		case 1:
			//if is leaf
			//ExitIndex = CurrentIndex;
			return BranchLength;
		case 2:
			//si hay 2 espacios vacios a los costados, sigo de largo
			NextNeighbor = GetNextNeighbor(CurrentIndex, LastIndex);
			LastIndex = CurrentIndex;
			CurrentIndex = NextNeighbor;
			BranchLength++;
			break;
		case 3:
			//si hay 3 espacios vacios, es una ramificacion, cuento ambas largos, los comparo, sumo a mi actual largo y devuelvo, empezando a retroceder en la recursividad
			GetNeighbors(CurrentIndex, LastIndex, N1, N2);
			if (SizeOfBranch(CurrentIndex, N1, N1LeafIndex) > SizeOfBranch(CurrentIndex, N2, N2LeafIndex))
			{
				//si el branch 1 nunca toco un borde, se descarta como ruta mas larga ya que no generara una salida
				if (N1LeafIndex != 0)
				{
					BranchLength = BranchLength + SizeOfBranch(CurrentIndex, N1, N1LeafIndex);
					ExitIndex = N1LeafIndex;
				}
				else
				{
					BranchLength = BranchLength + SizeOfBranch(CurrentIndex, N2, N2LeafIndex);
					ExitIndex = N2LeafIndex;
				}
			}
			else
			{
				//si el branch 2 nunca toco un borde, se descarta como ruta mas larga ya que no generara una salida
				if (N2LeafIndex != 0)
				{
					BranchLength = BranchLength + SizeOfBranch(CurrentIndex, N2, N2LeafIndex);
					ExitIndex = N2LeafIndex;
				}
				else
				{
					BranchLength = BranchLength + SizeOfBranch(CurrentIndex, N1, N1LeafIndex);
					ExitIndex = N1LeafIndex;
				}
			} 
			return BranchLength;
		}
	}
}

int EmptyNeighbors(int Index)
{
	int NeighborCount = 0;
	if ((!isBlock[Index + 1]) && isValidNeighbor(Index, Index + 1))
	{
		NeighborCount++;
	}
	if ((!isBlock[Index - 1]) && isValidNeighbor(Index, Index - 1))
	{
		NeighborCount++;
	}
	if ((!isBlock[Index + SideSize]) && isValidNeighbor(Index, Index + SideSize))
	{
		NeighborCount++;
	}
	if ((!isBlock[Index - SideSize]) && isValidNeighbor(Index, Index - SideSize))
	{
		NeighborCount++;
	}

	return NeighborCount;
}

void DrawMaze()
{
	for (int i = 0; i < SideSize + 2; i++)
	{
		cout << "*";
	}
	cout << "\n";

	for (int i = 0; i < SideSize; i++)
	{
		cout << "*";
		for (int j = 0; j < SideSize; j++)
		{
			if (ExitIndex == (i*SideSize + j))
			{
				cout << 'B';
			}
			else
			{
				if (0 == (i*SideSize + j))
				{
					cout << 'A';
				}
				else
				{
					if (isBlock[i*SideSize + j])
					{
						cout << '*';
					}
					else
					{
						cout << ' ';
					}
				}
			}
		}
		cout << "*";
		cout << "\n";
	}
	for (int i = 0; i < SideSize + 2; i++)
	{
		cout << "*";
	}
	cout << "\n";
}

void UpdateCells(int CurrentCell, int ToMoveCell) 
{
	isBlock[ToMoveCell] = false;
	isVisited[ToMoveCell] = true;

	if (isBackTracking)
	{
		isAlive[ToMoveCell] = false;
		LastVisitedCellIndex--;
	}
	else
	{
		isBlock[(ToMoveCell + CurrentCell) / 2] = false;
		isVisited[(ToMoveCell + CurrentCell) / 2] = true;
		isAlive[(ToMoveCell + CurrentCell) / 2] = false;
		VisitedCellIndexes[LastVisitedCellIndex] = ToMoveCell;
		LastVisitedCellIndex++;
	}
}

bool isValidNeighbor(int CurrentCell, int ToMoveCell)
{
	//se asume que CurrentCell siempre es diferente que ToMoveCell
	if ((ToMoveCell < 0) || (ToMoveCell >= SideSize*SideSize)) { return false; }
	else
	{
		//columna izq
		if ((CurrentCell % SideSize) == 0) 
		{
			//si me muevo a la derecha o el movimiento es vertical, es valido
			if ((ToMoveCell > CurrentCell) || (abs(ToMoveCell - CurrentCell) == SideSize)) { return true; }
			else { return false; }
		}
		if ((CurrentCell + 1) % SideSize == 0)
		{
			//si me muevo a la izquierda o el movimiento es vertical, es valido
			if ((CurrentCell > ToMoveCell) || (abs(ToMoveCell - CurrentCell) == SideSize)) { return true; }
			else { return false; }			
		}
		return true;
	}	
}

int PickRandomNeighbor(int CurrentCell)
{
	bool isEmpty = true;

	int CandidateNeighbors[4];
	fill_n(CandidateNeighbors, 4, -1);

	if (isValidNeighbor(CurrentCell, CurrentCell + 2) && isAlive[CurrentCell + 2] && !isVisited[CurrentCell + 2])
	{
		CandidateNeighbors[0] = CurrentCell + 2;
		isEmpty = false;
	}
	if (isValidNeighbor(CurrentCell, CurrentCell - 2) && isAlive[CurrentCell - 2] && !isVisited[CurrentCell - 2])
	{
		CandidateNeighbors[1] = CurrentCell - 2;
		isEmpty = false;
	}	
	if (isValidNeighbor(CurrentCell, CurrentCell + 2* SideSize) && isAlive[CurrentCell + 2 * SideSize] && !isVisited[CurrentCell + 2 * SideSize])
	{
		CandidateNeighbors[2] = CurrentCell + 2 * SideSize;
		isEmpty = false;
	}
	if (isValidNeighbor(CurrentCell, CurrentCell - 2 * SideSize) && isAlive[CurrentCell - 2 * SideSize] && !isVisited[CurrentCell - 2 * SideSize])
	{
		CandidateNeighbors[3] = CurrentCell - 2 * SideSize;
		isEmpty = false;
	}

	random_device rd;
	mt19937 rng(rd());
	uniform_int_distribution<int> uni(0, 3);

	auto rnd_integer = uni(rng);
	if (isEmpty)
	{
		isBackTracking = true;
		return -1;
	}
	else
	{
		isBackTracking = false;
		while (1)
		{
			rnd_integer = uni(rng);
			if (CandidateNeighbors[rnd_integer] != -1)
			{
				return CandidateNeighbors[rnd_integer];
			}
		}
	}
}

int GetNextNeighbor(int CurrentIndex, int LastIndex)
{
	///se invocara esta funcion siempre que haya solo 1 posible espacio a donde pasar
	if ((!isBlock[CurrentIndex + 1]) && isValidNeighbor(CurrentIndex,CurrentIndex + 1) && ((CurrentIndex + 1) != LastIndex)) { return (CurrentIndex + 1); }
	if ((!isBlock[CurrentIndex - 1]) && isValidNeighbor(CurrentIndex, CurrentIndex - 1) && ((CurrentIndex - 1) != LastIndex)) { return (CurrentIndex - 1); }
	if ((!isBlock[CurrentIndex + SideSize]) && isValidNeighbor(CurrentIndex, CurrentIndex + SideSize) && ((CurrentIndex + SideSize) != LastIndex)) { return (CurrentIndex + SideSize); }
	if ((!isBlock[CurrentIndex - SideSize]) && isValidNeighbor(CurrentIndex, CurrentIndex - SideSize) && ((CurrentIndex - SideSize) != LastIndex)) { return (CurrentIndex - SideSize); }
}

bool isBorder(int Index)
{
	return (((Index % SideSize) == 0) || ((Index + 1) % SideSize == 0) || (Index < SideSize) || (Index >= (SideSize*(SideSize - 1))));
}