// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialougeGraphRuntime.h"
#include "UObject/NoExportTypes.h"
#include "Dialouge.generated.h"

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
class DIALOUGENODEPLUGIN_API UDialouge : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY()
	UDialougeRuntimeGraph* Graph = nullptr;
};
