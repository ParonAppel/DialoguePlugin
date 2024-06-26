// Copyright Epic Games, Inc. All Rights Reserved.

#include "DialougeNodePluginEditor.h"

#define LOCTEXT_NAMESPACE "FDialougeNodePluginEditorModule"

void FDialougeNodePluginEditorModule::StartupModule()
{
	
	DialougeCategory = FAssetToolsModule::GetModule().Get().RegisterAdvancedAssetCategory(FName(TEXT("Dialouge")),LOCTEXT("Dialouge","Dialouge"));
	DialougeActions = MakeShared<FDialougeActions>();
	FAssetToolsModule::GetModule().Get().RegisterAssetTypeActions(DialougeActions.ToSharedRef());
}

void FDialougeNodePluginEditorModule::ShutdownModule()
{
	if (!FModuleManager::Get().IsModuleLoaded("AssetTools")) return;
	FAssetToolsModule::GetModule().Get().UnregisterAssetTypeActions(DialougeActions.ToSharedRef());
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDialougeNodePluginEditorModule, DialougeNodePluginEditor)