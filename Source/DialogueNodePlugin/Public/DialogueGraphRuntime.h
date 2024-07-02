// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueNodeSaveData.h"
#include "UObject/Object.h"
#include "DialogueGraphRuntime.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGUENODEPLUGIN_API UDialogueRuntimePin : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FName PinName;

	UPROPERTY()
	FGuid PinID;

	UPROPERTY()
	UDialogueRuntimePin* Connection = nullptr;

	UPROPERTY()
	UDialogueRuntimeNode* Owner = nullptr;
};

UCLASS()
class DIALOGUENODEPLUGIN_API UDialogueRuntimeNode : public UObject
{
	GENERATED_BODY()
public:

	UPROPERTY()
	FName NodeName;

	UPROPERTY()
	FColor NodeColor;
	
	UPROPERTY()
	UDialogueRuntimePin* InputPin = nullptr;

	UPROPERTY()
	TArray<UDialogueRuntimePin*> OutputPins;

	UPROPERTY()
	FVector2D Position;

	UPROPERTY()
	TSubclassOf<UEdGraphNode> NodeClass;

	UPROPERTY()
	UDialogueNodeSaveData* NodeSaveData;
};

UCLASS()
class DIALOGUENODEPLUGIN_API UDialogueRuntimeGraph : public UObject
{
	GENERATED_BODY()
public:
	
	UPROPERTY()
	TArray<UDialogueRuntimeNode*> Nodes;

	UPROPERTY()
	UDialogueRuntimeNode* StartNode = nullptr;
};


