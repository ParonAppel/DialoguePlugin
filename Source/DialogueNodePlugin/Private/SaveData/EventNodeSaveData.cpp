// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveData/EventNodeSaveData.h"
#include "UObject/Object.h"

#if WITH_EDITOR
void UEventNodeSaveData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif