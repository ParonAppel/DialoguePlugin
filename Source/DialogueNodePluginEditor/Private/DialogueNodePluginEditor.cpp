// Copyright Epic Games, Inc. All Rights Reserved.

#include "DialogueNodePluginEditor.h"

#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"

#define LOCTEXT_NAMESPACE "FDialogueNodePluginEditorModule"

void FDialogueNodePluginEditorModule::StartupModule()
{
	
	DialogueCategory = FAssetToolsModule::GetModule().Get().RegisterAdvancedAssetCategory(FName(TEXT("Dialogue")),LOCTEXT("Dialogue","Dialogue"));
	DialogueActions = MakeShared<FDialogueActions>();
	FAssetToolsModule::GetModule().Get().RegisterAssetTypeActions(DialogueActions.ToSharedRef());

	//Styling setup	
	StyleSet = MakeShareable(new FSlateStyleSet(TEXT("DialogueEditorStyle")));
	FString contentDir = IPluginManager::Get().FindPlugin("DialogueNodePlugin")->GetContentDir();
	StyleSet->SetContentRoot(contentDir);

	//Images
	FSlateImageBrush* addPinIconBrush  = new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("T_AddPin"), TEXT(".png")), FVector2D(256.0, 256.0));
	StyleSet->Set(TEXT("DialogueNodePluginEditor.AddPinIcon"),addPinIconBrush);

	FSlateImageBrush* removePinIconBrush  = new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("T_RemovePin"), TEXT(".png")), FVector2D(256.0, 256.0));
	StyleSet->Set(TEXT("DialogueNodePluginEditor.RemovePinIcon"),removePinIconBrush);

	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet);

	PinFactory = MakeShareable(new FDialoguePinFactory());
	FEdGraphUtilities::RegisterVisualPinFactory(PinFactory);
}

void FDialogueNodePluginEditorModule::ShutdownModule()
{
	FEdGraphUtilities::UnregisterVisualPinFactory(PinFactory);
	
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet);
	
	if (!FModuleManager::Get().IsModuleLoaded("AssetTools")) return;
	FAssetToolsModule::GetModule().Get().UnregisterAssetTypeActions(DialogueActions.ToSharedRef());
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDialogueNodePluginEditorModule, DialogueNodePluginEditor)