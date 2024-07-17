// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueNodeSaveData.h"
#include "PlayerOptionsSaveData.generated.h"

/**
 * 
 */

USTRUCT(Blueprintable)
struct FPlayerOptions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText OptionsText = INVTEXT("Option");
	
};
UCLASS()
class DIALOGUENODEPLUGIN_API UPlayerOptionsSaveData : public UDialogueNodeSaveData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FPlayerOptions> PlayerOptions;
};
