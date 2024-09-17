// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueNodeSaveData.h"

#if WITH_EDITOR
void UDialogueNodeSaveData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	OnPropertyChanged.Broadcast();
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
