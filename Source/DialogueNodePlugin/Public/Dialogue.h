// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueGraphRuntime.h"
#include "UObject/NoExportTypes.h"
#include "Dialogue.generated.h"

/**
 * 
 */
USTRUCT(Blueprintable)
struct FCastMember  : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name = INVTEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor Color = FColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> Portrait = nullptr;
};

UCLASS()
class DIALOGUENODEPLUGIN_API UDialogue : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY()
	UDialogueRuntimeGraph* Graph = nullptr;
};
