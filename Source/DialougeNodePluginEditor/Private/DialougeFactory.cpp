// Fill out your copyright notice in the Description page of Project Settings.


#include "DialougeFactory.h"

#include "Dialouge.h"

UDialougeFactory::UDialougeFactory()
{
	SupportedClass = UDialouge::StaticClass();
	bCreateNew = true;
}

UObject* UDialougeFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
                                            UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UDialouge>(InParent, InClass, InName, Flags, Context);
}
