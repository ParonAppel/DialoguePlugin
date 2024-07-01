// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DialougeNodeSaveData.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPropertyChangedSignature)
/**
 * 
 */
UCLASS()
class DIALOUGENODEPLUGIN_API UDialougeNodeSaveData : public UObject
{
	GENERATED_BODY()

public:

	UDialougeNodeSaveData();

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	FOnPropertyChangedSignature OnPropertyChanged;
	
};
