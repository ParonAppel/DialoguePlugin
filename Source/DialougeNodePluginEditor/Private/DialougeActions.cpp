// Fill out your copyright notice in the Description page of Project Settings.


#include "DialougeActions.h"

#include "DialougeEditorToolkit.h"
#include "DialougeNodePlugin/Public/Dialouge.h"

UClass* FDialougeActions::GetSupportedClass() const
{
	return UDialouge::StaticClass();
}

FText FDialougeActions::GetName() const
{
	return INVTEXT("Dialouge Graph");
}

FColor FDialougeActions::GetTypeColor() const
{
	return FColor::Cyan;
}

uint32 FDialougeActions::GetCategories()
{
	return EAssetTypeCategories::Gameplay;
}

void FDialougeActions::OpenAssetEditor(const TArray<UObject*>& InObjects,
	TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	MakeShared<FDialougeEditorToolkit>()->InitEditor(InObjects);
}
