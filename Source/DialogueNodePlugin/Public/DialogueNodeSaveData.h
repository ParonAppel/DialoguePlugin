// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DialogueNodeSaveData.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPropertyChangedSignature)
/**
 * 
 */
UCLASS()
class DIALOGUENODEPLUGIN_API UDialogueNodeSaveData : public UObject
{
	GENERATED_BODY()

public:

	UDialogueNodeSaveData();

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	FOnPropertyChangedSignature OnPropertyChanged;
	
};
