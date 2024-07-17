// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SaveData/PlayerOptionsSaveData.h"
#include "DialogueWidgetBase.generated.h"

struct FCastMember;
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNextDialogueSignature, int, OptionsAnswer);

UCLASS()
class DIALOGUENODEPLUGIN_API UDialogueWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent)
	void OnNewDialogue(FCastMember Cast, const FText& DialogueText);

	UFUNCTION(BlueprintImplementableEvent)
	void OnNewOptions(const TMap<int ,FPlayerOptions>& Options);

	UFUNCTION(BlueprintCallable)
	void CallOnNextDialogue();

	UFUNCTION(BlueprintCallable)
	void SendOptionsAnswer(int OptionChosen);
	
	UPROPERTY()
	FOnNextDialogueSignature OnNextDialogue;



protected:
	
};
