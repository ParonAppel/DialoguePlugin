// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeCategories.h"
#include "Factories/Factory.h"
#include "DialogueFactory.generated.h"

/**
 * 
 */

UCLASS()
class DIALOGUENODEPLUGINEDITOR_API UDialogueFactory : public UFactory
{
	GENERATED_BODY()
	
public:

	UDialogueFactory();
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
