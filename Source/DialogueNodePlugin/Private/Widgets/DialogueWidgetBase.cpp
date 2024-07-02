// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/DialogueWidgetBase.h"

void UDialogueWidgetBase::CallOnNextDialogue()
{
	OnNextDialogue.Broadcast();
}
