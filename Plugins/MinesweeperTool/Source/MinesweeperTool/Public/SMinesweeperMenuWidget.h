// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"

/**
 * 
 */
class MinesweeperManager;

class SMinesweeperMenuWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SMinesweeperMenuWidget) {}
	SLATE_ARGUMENT(TWeakObjectPtr<class AMenuHUD>,OwningHUD)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	TWeakObjectPtr<class AMenuHUD> OwningHUD;

	TSharedPtr<SEditableTextBox> WidthBox;
	TSharedPtr<SEditableTextBox> HeightBox;
	TSharedPtr<SEditableTextBox> MinesAmountBox;
	TSharedPtr<SUniformGridPanel> Board;
	TSharedPtr<STextBlock> EndingText;
	MinesweeperManager* Minesweeper;

	
	void UpdateMinesweeperBoard();
	FReply OnGenerateClicked();
	FReply OnSlotClicked(int32 Index);

	virtual bool SupportsKeyboardFocus() const override { return true; }
};
