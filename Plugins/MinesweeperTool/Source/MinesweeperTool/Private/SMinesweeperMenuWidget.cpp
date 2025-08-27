

#include "SMinesweeperMenuWidget.h"
#include "MinesweeperManager.h"

#define LOCTEXT_NAMESPACE "Minesweeper"

void SMinesweeperMenuWidget::Construct(const FArguments& InArgs) {
	
	Minesweeper = new MinesweeperManager();
	bCanSupportFocus = true;
	OwningHUD = InArgs._OwningHUD;

	const FText TitleText = LOCTEXT("TitleText", "Minesweeper Task");
	const FText WidthText = LOCTEXT("WidthText", "Width: ");
	const FText HeightText = LOCTEXT("HeightText", "Height: ");
	const FText MinesAmountText = LOCTEXT("MinesAmountText", "Number Of Mines: ");
	const FText GenerateButtonText = LOCTEXT("GenerateButtonText", "Generate new grid");
	
	FSlateFontInfo FontStyle = FCoreStyle::Get().GetFontStyle("EmbossedText");
	FontStyle.Size = 40;

	ChildSlot [
			SNew(SOverlay) 
				// Background
				+ SOverlay::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Fill)[
				SNew(SImage).ColorAndOpacity(FColor::Black)
			]
				+ SOverlay::Slot().HAlign(HAlign_Center).VAlign(VAlign_Fill).Padding(FMargin(40.f, 20.f))[
					SNew(SVerticalBox) 
						// Game Title
						+ SVerticalBox::Slot().AutoHeight().Padding(10)[
					SNew(STextBlock).Text(TitleText).Font(FSlateFontInfo(FontStyle))
						]
						+ SVerticalBox::Slot().AutoHeight().Padding(10)[
							SNew(SHorizontalBox)
								// Width Text
								+ SHorizontalBox::Slot().AutoWidth().Padding(5)[
									SNew(STextBlock).Text(WidthText)
								]
								// Width Input
								+ SHorizontalBox::Slot().AutoWidth().Padding(5)[
									SAssignNew(WidthBox, SEditableTextBox).MinDesiredWidth(80).Padding(FMargin(5, 2, 5, 2))
								]
								// Height Text
								+ SHorizontalBox::Slot().AutoWidth().Padding(5)[
									SNew(STextBlock).Text(HeightText)
								]
								// Height Input
								+ SHorizontalBox::Slot().AutoWidth().Padding(5)[
									SAssignNew(HeightBox, SEditableTextBox).MinDesiredWidth(80).Padding(FMargin(5, 2, 5, 2))
								]
						]
						+ SVerticalBox::Slot().AutoHeight().Padding(10)[
							SNew(SHorizontalBox)
								//MinesAmount Text
								+ SHorizontalBox::Slot().AutoWidth().Padding(5)[
									SNew(STextBlock).Text(MinesAmountText)
								]
								//MinesAmount Input
								+ SHorizontalBox::Slot().AutoWidth().Padding(5)[
									SAssignNew(MinesAmountBox, SEditableTextBox).MinDesiredWidth(80).Padding(FMargin(5, 2, 5, 2))
								]
						]
						+ SVerticalBox::Slot().AutoHeight().Padding(10)[
							// Generate Button
							SNew(SButton).Text(GenerateButtonText).OnClicked(this, &SMinesweeperMenuWidget::OnGenerateClicked)
								]
						+ SVerticalBox::Slot().AutoHeight().Padding(10)[
							// Empty Board
							SAssignNew(Board, SUniformGridPanel)
								]
						+ SVerticalBox::Slot().AutoHeight().Padding(10)[
							// Place For Ending Text
							SAssignNew(EndingText, STextBlock).Justification(ETextJustify::Center).Font(FontStyle)
								]

				
			]
		];
}
void SMinesweeperMenuWidget::UpdateMinesweeperBoard()
{
	if (!Board.IsValid() || !Minesweeper || Minesweeper->MinesweeperBoard.size() != Minesweeper->Width * Minesweeper->Height) 
		return;

	Board->ClearChildren(); 

	int32 Width = Minesweeper->Width;
	int32 Height = Minesweeper->Height;

	//O(n) n=Width*Height
	for (int32 X = 0; X < Width; ++X)
	{
		for (int32 Y = 0; Y < Height; ++Y)
		{
			int32 Index = Y * Width + X;
			int32 SlotValue = Minesweeper->MinesweeperBoard[Index].NumberOfMinesAround >= -1 
				? Minesweeper->MinesweeperBoard[Index].NumberOfMinesAround : -1;

			FLinearColor ButtonColor = FLinearColor::Green;
			bool bIsClickable = false;
			FText DisplayText = FText::GetEmpty();

			if (SlotValue == -1)
			{
				ButtonColor = FLinearColor::Black;
				bIsClickable = true;
			}
			else if(SlotValue != 0)
				DisplayText = FText::AsNumber(SlotValue);

			// Create the button
			TSharedRef<SButton> Button = SNew(SButton).ButtonColorAndOpacity(ButtonColor).IsEnabled(bIsClickable)
				.OnClicked_Lambda([this, Index]() { return OnSlotClicked(Index); })[
					SNew(STextBlock).Text(DisplayText).Justification(ETextJustify::Center)
				];

			Board->AddSlot(X, Y)[Button];
		}
	}
}
FReply SMinesweeperMenuWidget::OnGenerateClicked()
{
	if (!Minesweeper)
	{
		UE_LOG(LogTemp, Display, TEXT("Minesweeper is nullptr"));
		return FReply::Handled();
	}

	int32 Width = FCString::Atoi(*WidthBox->GetText().ToString());
	int32 Height = FCString::Atoi(*HeightBox->GetText().ToString());
	int32 MinesAmount = FCString::Atoi(*MinesAmountBox->GetText().ToString());

	EndingText->SetText(FText::FromString(""));
	Minesweeper->GenerateMinesweeperBoard(Width, Height, MinesAmount);

	UpdateMinesweeperBoard();
	return FReply::Handled();
}

FReply SMinesweeperMenuWidget::OnSlotClicked(int32 Index)
{
	if (Minesweeper->bIsFirstSlot)
		Minesweeper->FirstStep(Index);
	else
		Minesweeper->StepOnASlot(Index);
	if (Minesweeper->MinesweeperBoard.size() == 0 || Minesweeper->MinesweeperBoard.size() != Minesweeper->Width * Minesweeper->Height)
		return FReply::Handled();
	if(Minesweeper->MinesweeperBoard[Index].NumberOfMinesAround == -1){
		EndingText->SetText(FText::FromString("Game Over"));
		EndingText->SetColorAndOpacity(FLinearColor::Red);
		Minesweeper->IsGameActive = false;
	}
	else if (Minesweeper->DidPlayerWin()){
		EndingText->SetText(FText::FromString("You Win!"));
		EndingText->SetColorAndOpacity(FLinearColor::Green);
		Minesweeper->IsGameActive = false;
	}
	UpdateMinesweeperBoard();
	return FReply::Handled();
}


#undef LOCTEXT_NAMESPACE


