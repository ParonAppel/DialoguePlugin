// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DialogueComponent.h"

#include "SaveData/SimpleDialougeSaveData.h"


// Sets default values for this component's properties
UDialogueComponent::UDialogueComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UDialogueComponent::OnNextDialogue()
{
	if(!CurrentNode  || !CurrentNode->OutputPins[0]->Connection)
	{
		QuitDialouge();
		return;
	}
	if(!CurrentNode->OutputPins[0]->Connection->Owner)
	{
		QuitDialouge();
		return;
	}

	CurrentNode = CurrentNode->OutputPins[0]->Connection->Owner;
	
	if(CurrentNode->NodeSaveData->GetClass() == USimpleDialougeSaveData::StaticClass())
	{
		USimpleDialougeSaveData* saveData = Cast<USimpleDialougeSaveData>(CurrentNode->NodeSaveData);
		DialogueWidget->OnNewDialouge(saveData->CastMember, saveData->DialougeText);
	}
	
	
}

// Called when the game starts
void UDialogueComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDialogueComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDialogueComponent::StartDialouge()
{
	if(DialogueWidget)
		return;
	
	CurrentNode = Dialouge->Graph->StartNode->OutputPins[0]->Connection->Owner;

	if(!CurrentNode)
	{
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No Dialouge"));
		
		return;
	}
		
	if(!DialogueWidgetClass)
		return;
	
	DialogueWidget = Cast<UDialogueWidgetBase>(CreateWidget(GetWorld(), DialogueWidgetClass));
	DialogueWidget->AddToViewport(0);

	DialogueWidget->OnNextDialogue.AddUniqueDynamic(this, &UDialogueComponent::OnNextDialogue);

	if(CurrentNode->NodeSaveData->GetClass() == USimpleDialougeSaveData::StaticClass())
	{
		USimpleDialougeSaveData* saveData = Cast<USimpleDialougeSaveData>(CurrentNode->NodeSaveData);
		DialogueWidget->OnNewDialouge(saveData->CastMember, saveData->DialougeText);
	}
	
}

void UDialogueComponent::QuitDialouge()
{
	DialogueWidget->OnNextDialogue.RemoveDynamic(this, &UDialogueComponent::OnNextDialogue);
	DialogueWidget->RemoveFromParent();
	DialogueWidget = nullptr;
}

