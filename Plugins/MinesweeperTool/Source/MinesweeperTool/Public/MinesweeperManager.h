// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <vector>
#include "MinesweeperSlot.h"
/**
 * 
 */
class MINESWEEPERTOOL_API  MinesweeperManager
{
public:
	MinesweeperManager();
	~MinesweeperManager();

	std::vector<MinesweeperSlot> MinesweeperBoard;

	int32 Width;
	int32 Height;
	int32 MinesAmount;
	bool bIsFirstSlot = true;
	bool IsGameActive = false;

	bool GenerateMinesweeperBoard(int32 InWidth, int32 InHeight, int32 InMinesAmount);

	void ResetMinesweeperBoard();

	void FirstStep(int32 indIndexex);

	void StepOnASlot(int32 Index);

	int32 CountNeighboringSlots(int32 Index) const;

	void RevealSlot(int32 Index);

	bool DidPlayerWin();

};
