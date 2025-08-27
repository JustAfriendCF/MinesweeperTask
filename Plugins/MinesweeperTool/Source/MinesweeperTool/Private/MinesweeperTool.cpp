// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinesweeperTool.h"
#include "MinesweeperToolStyle.h"
#include "MinesweeperToolCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "SMinesweeperMenuWidget.h"

static const FName MinesweeperToolTabName("MinesweeperTool");

#define LOCTEXT_NAMESPACE "FMinesweeperToolModule"

TSharedRef<SDockTab> FMinesweeperToolModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SMinesweeperMenuWidget)
		];
}

void FMinesweeperToolModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FMinesweeperToolStyle::Initialize();
	FMinesweeperToolStyle::ReloadTextures();

	FMinesweeperToolCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FMinesweeperToolCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FMinesweeperToolModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMinesweeperToolModule::RegisterMenus));

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner("MinesweeperTab",
		FOnSpawnTab::CreateRaw(this, &FMinesweeperToolModule::OnSpawnPluginTab))
		.SetDisplayName(FText::FromString("Minesweeper")).SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FMinesweeperToolModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner("MinesweeperTab");

	FMinesweeperToolStyle::Shutdown();

	FMinesweeperToolCommands::Unregister();
}

void FMinesweeperToolModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	/*FText DialogText = FText::Format(
							LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
							FText::FromString(TEXT("FMinesweeperToolModule::PluginButtonClicked()")),
							FText::FromString(TEXT("MinesweeperTool.cpp"))
					   );
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);*/

	FGlobalTabmanager::Get()->TryInvokeTab(FTabId("MinesweeperTab"));
}

void FMinesweeperToolModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FMinesweeperToolCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FMinesweeperToolCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMinesweeperToolModule, MinesweeperTool)