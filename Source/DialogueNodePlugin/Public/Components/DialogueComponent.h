// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dialogue.h"
#include "Components/ActorComponent.h"
#include "Widgets/DialogueWidgetBase.h"
#include "DialogueComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIALOGUENODEPLUGIN_API UDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDialogueComponent();
	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

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

protected:

	UPROPERTY(BlueprintReadWrite)
	UDialogueWidgetBase* DialogueWidget;
	
	virtual void BeginPlay() override;

	UPROPERTY()
	UDialogueRuntimeNode* CurrentNode = nullptr;

public:
	
};
