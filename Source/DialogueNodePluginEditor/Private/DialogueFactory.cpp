// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueFactory.h"

#include "Dialogue.h"

UDialogueFactory::UDialogueFactory()
{
	SupportedClass = UDialogue::StaticClass();
	bCreateNew = true;
}

UObject* UDialogueFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
                                            UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UDialogue>(InParent, InClass, InName, Flags, Context);
}
