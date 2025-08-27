// Fill out your copyright notice in the Description page of Project Settings.

#include "MinesweeperManager.h"
#include "MinesweeperSlot.h"
#include <random>
#include "Math/UnrealMathUtility.h"

constexpr int32 WAS_NOT_SEARCHED = -1;

MinesweeperManager::MinesweeperManager()
    : Width(0)
    , Height(0)
    , MinesAmount(0)
{
}

MinesweeperManager::~MinesweeperManager()
{
    ResetMinesweeperBoard();
}

bool MinesweeperManager::GenerateMinesweeperBoard(int32 InWidth, int32 InHeight, int32 InMinesAmount)
{
    ResetMinesweeperBoard();

    if (InWidth <= 0 || InHeight <= 0 || InMinesAmount <= 0 || InMinesAmount >= InWidth * InHeight) {
        UE_LOG(LogTemp, Error, TEXT("Invalid board parameters"));
        return false;
    }

    Width = InWidth;
    Height = InHeight;
    MinesAmount = InMinesAmount;
    MinesweeperBoard.resize(Width * Height);

    return true;
}

void MinesweeperManager::ResetMinesweeperBoard()
{
    MinesweeperBoard.clear();
    
    Width = 0;
    Height = 0;
    MinesAmount = 0;
    bIsFirstSlot = true;
    IsGameActive = true;
}

void MinesweeperManager::FirstStep(int32 Index)
{

    if (Index < 0 || Index >= MinesweeperBoard.size())
        return;

    std::vector<int32> RandomSlots;
    //O(n)
    for (size_t i = 0; i < MinesweeperBoard.size(); i++)
        if(i != Index)
            RandomSlots.push_back(i);

    std::random_device rd;
    std::mt19937 generator(rd());

	//O(n)
    for (size_t i = 0; i < MinesweeperBoard.size(); i++)
        std::shuffle(RandomSlots.begin(), RandomSlots.end(), generator);

    //O(n)
    for (size_t i = 0; i < MinesAmount; i++)
        MinesweeperBoard[RandomSlots[i]].IsMine = true;

    bIsFirstSlot=false;
    RevealSlot(Index);
}
void MinesweeperManager::StepOnASlot(int32 Index)
{
    if (Index < 0 || Index >= MinesweeperBoard.size() || MinesweeperBoard[Index].NumberOfMinesAround != WAS_NOT_SEARCHED || !IsGameActive)
        return;

    // Mine was stepped on, GameOver :(
    if (MinesweeperBoard[Index].IsMine)
		return;

    RevealSlot(Index);
}

int MinesweeperManager::CountNeighboringSlots(int32 Index) const
{
    const int32 RowIndex = Index / Width;
    const int32 ColumnIndex = Index % Width;

    int32 MinesCounter = 0;

    //O(1)
    for (int32 i = -1; i <= 1; i++)
        for (int32 j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0) continue;

            const int32 NighborX = RowIndex + i;
            const int32 NighborY = ColumnIndex + j;

            if (NighborX >= 0 && NighborX < Height && NighborY >= 0 && NighborY < Width &&
                MinesweeperBoard[NighborX * Width + NighborY].IsMine)
                MinesCounter++;
        }
   
    return MinesCounter;
}
void MinesweeperManager::RevealSlot(int32 Index)
{
    int32 NeighboringMinesAmout = CountNeighboringSlots(Index);
        MinesweeperBoard[Index].NumberOfMinesAround = NeighboringMinesAmout;

    if (NeighboringMinesAmout != 0)
        return;
    
    const int32 RowIndex = Index / Width;
    const int32 ColumnIndex = Index % Width;

    //O(n)
    for (int32 i = -1; i <= 1; i++)
        for (int32 j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0) continue;

            const int32 NighborX = RowIndex + i;
            const int32 NighborY = ColumnIndex + j;

            if (NighborX >= 0 && NighborX < Height && NighborY >= 0 && NighborY < Width &&
                MinesweeperBoard[NighborX * Width + NighborY].NumberOfMinesAround == WAS_NOT_SEARCHED)
                RevealSlot(NighborX * Width + NighborY);
        }
}

bool MinesweeperManager::DidPlayerWin()
{
    //O(n)
    for (auto& slot : MinesweeperBoard)
		if (!slot.IsMine && slot.NumberOfMinesAround == WAS_NOT_SEARCHED)
			return false;
    return true;
}


