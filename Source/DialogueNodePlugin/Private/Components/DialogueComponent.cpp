// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DialogueComponent.h"

#include "SaveData/PlayerOptionsSaveData.h"
#include "SaveData/SimpleDialogueSaveData.h"


// Sets default values for this component's properties
UDialogueComponent::UDialogueComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UDialogueComponent::OnNextDialogue(int Pin)
{
	if(!CurrentNode  || !CurrentNode->OutputPins[Pin]->Connection)
	{
		QuitDialogue();
		return;
	}
	if(!CurrentNode->OutputPins[Pin]->Connection->Owner)
	{
		QuitDialogue();
		return;
	}

	CurrentNode = CurrentNode->OutputPins[Pin]->Connection->Owner;
	
	if(CurrentNode->NodeSaveData->GetClass() == USimpleDialogueSaveData::StaticClass())
	{
		USimpleDialogueSaveData* saveData = Cast<USimpleDialogueSaveData>(CurrentNode->NodeSaveData);
		DialogueWidget->OnNewDialogue(saveData->CastMember, saveData->DialogueText);
	}
	else if(CurrentNode->NodeSaveData->GetClass() == UPlayerOptionsSaveData::StaticClass())
	{
		UPlayerOptionsSaveData* saveData = Cast<UPlayerOptionsSaveData>(CurrentNode->NodeSaveData);
		TMap<int, FPlayerOptions> optionsToSend;
		for (int i = 0; i <  saveData->PlayerOptions.Num(); i++)
		{
			// TODO: Implement logic to rule out options
			optionsToSend.Add(i,saveData->PlayerOptions[i]);
		}
		DialogueWidget->OnNewOptions(optionsToSend);
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

void UDialogueComponent::StartDialogue()
{
	if(DialogueWidget || !Dialogue || !DialogueWidgetClass)
		return;
	
	CurrentNode = Dialogue->Graph->StartNode;

	if(!CurrentNode)
	{
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No Dialogue"));
		
		return;
	}
	
	DialogueWidget = Cast<UDialogueWidgetBase>(CreateWidget(GetWorld(), DialogueWidgetClass));
	DialogueWidget->AddToViewport(0);

	DialogueWidget->OnNextDialogue.AddUniqueDynamic(this, &UDialogueComponent::OnNextDialogue);

	OnNextDialogue(0);
}

void UDialogueComponent::QuitDialogue()
{
	DialogueWidget->OnNextDialogue.RemoveDynamic(this, &UDialogueComponent::OnNextDialogue);
	DialogueWidget->RemoveFromParent();
	DialogueWidget = nullptr;
}

