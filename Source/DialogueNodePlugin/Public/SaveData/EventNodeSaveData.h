// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueNodeSaveData.h"
#include "EventNodeSaveData.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGUENODEPLUGIN_API UEventNodeSaveData : public UDialogueNodeSaveData
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cast");
	FName EventName;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
};
