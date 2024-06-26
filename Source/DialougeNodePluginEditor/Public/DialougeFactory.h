// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeCategories.h"
#include "Factories/Factory.h"
#include "DialougeFactory.generated.h"

/**
 * 
 */

UCLASS()
class DIALOUGENODEPLUGINEDITOR_API UDialougeFactory : public UFactory
{
	GENERATED_BODY()
	
public:

	UDialougeFactory();
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
