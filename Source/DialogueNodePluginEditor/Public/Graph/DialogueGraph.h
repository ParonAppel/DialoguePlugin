// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dialogue.h"
#include "EdGraph/EdGraph.h"
#include "DialogueGraph.generated.h"

/**
 * 
 */


UCLASS()
class DIALOGUENODEPLUGINEDITOR_API UDialogueGraph : public UEdGraph
{
	GENERATED_BODY()
public:

	UPROPERTY()
	UDialogue* DialogueAsset = nullptr;
};
