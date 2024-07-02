// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueActions.h"

#include "DialogueEditorToolkit.h"
#include "DialogueNodePlugin/Public/Dialogue.h"

UClass* FDialogueActions::GetSupportedClass() const
{
	return UDialogue::StaticClass();
}

FText FDialogueActions::GetName() const
{
	return INVTEXT("Dialogue Graph");
}

FColor FDialogueActions::GetTypeColor() const
{
	return FColor::Cyan;
}

uint32 FDialogueActions::GetCategories()
{
	return EAssetTypeCategories::Gameplay;
}

void FDialogueActions::OpenAssetEditor(const TArray<UObject*>& InObjects,
	TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	MakeShared<FDialogueEditorToolkit>()->InitEditor(InObjects);
}
