// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueNodeSaveData.h"

UDialogueNodeSaveData::UDialogueNodeSaveData()
{
}

void UDialogueNodeSaveData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	OnPropertyChanged.Broadcast();
	UObject::PostEditChangeProperty(PropertyChangedEvent);
}
