// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dialouge.h"
#include "EdGraph/EdGraph.h"
#include "DialougeGraph.generated.h"

/**
 * 
 */


UCLASS()
class DIALOUGENODEPLUGINEDITOR_API UDialougeGraph : public UEdGraph
{
	GENERATED_BODY()
public:

	UPROPERTY()
	UDialouge* DialougeAsset = nullptr;
};
