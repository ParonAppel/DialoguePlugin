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

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Instanced)
	TObjectPtr<UPlayerOptionModifier> Modifier;
	
};
UCLASS()
class DIALOGUENODEPLUGIN_API UPlayerOptionsSaveData : public UDialogueNodeSaveData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FPlayerOptions> PlayerOptions;
};

UCLASS(Blueprintable, Abstract, EditInlineNew, DefaultConfig)
class DIALOGUENODEPLUGIN_API UPlayerOptionModifier : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	bool CheckIfOptionIsAvailable();
};
