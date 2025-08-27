// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class MINESWEEPERTASK_API AMenuHUD : public AHUD
{
	GENERATED_BODY()

protected:

	TSharedPtr<class SMinesweeperMenuWidget> MenuWidget;
	TSharedPtr<class SWidget> MenuWigetContainer;

	virtual void BeginPlay() override;
};
