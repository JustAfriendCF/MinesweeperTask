// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
struct MINESWEEPERTOOL_API MinesweeperSlot
{
public:

	bool IsMine = false;
	int NumberOfMinesAround = -1;
};
