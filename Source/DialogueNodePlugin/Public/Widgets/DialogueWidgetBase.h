// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueWidgetBase.generated.h"

struct FCastMember;
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNextDialogueSignature);

UCLASS()
class DIALOGUENODEPLUGIN_API UDialogueWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent)
	void OnNewDialogue(FCastMember Cast, const FText& DialogueText);

	UFUNCTION(BlueprintCallable)
	void CallOnNextDialogue();
	
	UPROPERTY()
	FOnNextDialogueSignature OnNextDialogue;

protected:
	
};
