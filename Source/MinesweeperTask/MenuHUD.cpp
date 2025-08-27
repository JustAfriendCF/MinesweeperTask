// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuHUD.h"
#include "Widgets/SweakWidget.h"
#include "Engine/Engine.h"

void AMenuHUD::BeginPlay()
{
	Super::BeginPlay();
/*
	if(GEngine && GEngine->GameViewport)
	{
		MenuWidget = SNew(SMinesweeperMenuWidget).OwningHUD(this);
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(MenuWigetContainer, SWeakWidget).PossiblyNullContent(MenuWidget.ToSharedRef()));
	
        APlayerController* PC = GetWorld()->GetFirstPlayerController();
        if (PC)
        {
            PC->bShowMouseCursor = true;
            FInputModeGameAndUI InputMode;
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            InputMode.SetWidgetToFocus(nullptr);
            PC->SetInputMode(InputMode);
        }
	}*/
}
