// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DialougeGraphRuntime.generated.h"

/**
 * 
 */
UCLASS()
class DIALOUGENODEPLUGIN_API UDialougeRuntimePin : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FName PinName;

	UPROPERTY()
	FGuid PinID;

	UPROPERTY()
	UDialougeRuntimePin* Connection = nullptr;
};

UCLASS()
class DIALOUGENODEPLUGIN_API UDialougeRuntimeNode : public UObject
{
	GENERATED_BODY()
public:

	UPROPERTY()
	FName NodeName;

	UPROPERTY()
	FColor NodeColor;
	
	UPROPERTY()
	UDialougeRuntimePin* InputPin = nullptr;

	UPROPERTY()
	TArray<UDialougeRuntimePin*> OutputPins;

	UPROPERTY()
	FVector2D Position;
};

UCLASS()
class DIALOUGENODEPLUGIN_API UDialougeRuntimeGraph : public UObject
{
	GENERATED_BODY()
public:
	
	UPROPERTY()
	TArray<UDialougeRuntimeNode*> Nodes;


};