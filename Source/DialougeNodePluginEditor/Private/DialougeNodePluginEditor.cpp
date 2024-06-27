// Copyright Epic Games, Inc. All Rights Reserved.

#include "DialougeNodePluginEditor.h"

#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"

#define LOCTEXT_NAMESPACE "FDialougeNodePluginEditorModule"

void FDialougeNodePluginEditorModule::StartupModule()
{
	
	DialougeCategory = FAssetToolsModule::GetModule().Get().RegisterAdvancedAssetCategory(FName(TEXT("Dialouge")),LOCTEXT("Dialouge","Dialouge"));
	DialougeActions = MakeShared<FDialougeActions>();
	FAssetToolsModule::GetModule().Get().RegisterAssetTypeActions(DialougeActions.ToSharedRef());

	//Styling setup	
	StyleSet = MakeShareable(new FSlateStyleSet(TEXT("DialougeEditorStyle")));
	FString contentDir = IPluginManager::Get().FindPlugin("DialougeNodePlugin")->GetContentDir();
	StyleSet->SetContentRoot(contentDir);

	//Images
	FSlateImageBrush* addPinIconBrush  = new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("T_AddPin"), TEXT(".png")), FVector2D(256.0, 256.0));
	StyleSet->Set(TEXT("DialougeNodePluginEditor.AddPinIcon"),addPinIconBrush);

	FSlateImageBrush* removePinIconBrush  = new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("T_RemovePin"), TEXT(".png")), FVector2D(256.0, 256.0));
	StyleSet->Set(TEXT("DialougeNodePluginEditor.RemovePinIcon"),removePinIconBrush);

	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet);

	PinFactory = MakeShareable(new FDialougePinFactory());
	FEdGraphUtilities::RegisterVisualPinFactory(PinFactory);
}

void FDialougeNodePluginEditorModule::ShutdownModule()
{
	FEdGraphUtilities::UnregisterVisualPinFactory(PinFactory);
	
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet);
	
	if (!FModuleManager::Get().IsModuleLoaded("AssetTools")) return;
	FAssetToolsModule::GetModule().Get().UnregisterAssetTypeActions(DialougeActions.ToSharedRef());
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDialougeNodePluginEditorModule, DialougeNodePluginEditor)