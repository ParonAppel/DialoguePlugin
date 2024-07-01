// Fill out your copyright notice in the Description page of Project Settings.


#include "DialougeNodeSaveData.h"

UDialougeNodeSaveData::UDialougeNodeSaveData()
{
}

void UDialougeNodeSaveData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	OnPropertyChanged.Broadcast();
	UObject::PostEditChangeProperty(PropertyChangedEvent);
}
