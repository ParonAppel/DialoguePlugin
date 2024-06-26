// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Dialouge.generated.h"

/**
 * 
 */
UCLASS()
class DIALOUGENODEPLUGIN_API UDialouge : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	float Number;
};
