// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueNodeSaveData.h"

void UDialogueNodeSaveData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	OnPropertyChanged.Broadcast();
	UObject::PostEditChangeProperty(PropertyChangedEvent);
}
