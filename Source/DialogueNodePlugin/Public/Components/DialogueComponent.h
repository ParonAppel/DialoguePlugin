// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dialogue.h"
#include "Components/ActorComponent.h"
#include "Widgets/DialogueWidgetBase.h"
#include "DialogueComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialougeTreeCompletedSignature);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIALOGUENODEPLUGIN_API UDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDialogueComponent();
	
	UFUNCTION(BlueprintCallable)
	void StartDialogue();

	UFUNCTION()
	void QuitDialogue();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UDialogue> Dialogue = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDialogueWidgetBase> DialogueWidgetClass;

	UFUNCTION()
	void OnNextDialogue(int Pin);

	UPROPERTY(BlueprintAssignable)
	FOnDialougeTreeCompletedSignature OnDialougeTreeCompleted;
	
protected:

	UPROPERTY(BlueprintReadWrite)
	UDialogueWidgetBase* DialogueWidget;
	
	UPROPERTY()
	UDialogueRuntimeNode* CurrentNode = nullptr;

private:

	void HandlePlayerOptions();
	void HandleTagBranch();
	void HandleEventNode();
	
};
