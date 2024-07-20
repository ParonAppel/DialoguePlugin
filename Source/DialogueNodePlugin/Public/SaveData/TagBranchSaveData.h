// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueNodeSaveData.h"
#include "GameplayTagContainer.h"
#include "TagBranchSaveData.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGUENODEPLUGIN_API UTagBranchSaveData : public UDialogueNodeSaveData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer NecessaryTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer ExcludedTags;

};
