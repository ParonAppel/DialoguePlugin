// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dialouge.h"
#include "Components/ActorComponent.h"
#include "Widgets/DialogueWidgetBase.h"
#include "DialogueComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIALOUGENODEPLUGIN_API UDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDialogueComponent();
	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void StartDialouge();

	UFUNCTION()
	void QuitDialouge();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UDialouge> Dialouge = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDialogueWidgetBase> DialogueWidgetClass;

	UFUNCTION()
	void OnNextDialogue();

protected:

	UPROPERTY(BlueprintReadWrite)
	UDialogueWidgetBase* DialogueWidget;
	
	virtual void BeginPlay() override;

	UPROPERTY()
	UDialougeRuntimeNode* CurrentNode = nullptr;

public:
	
};
