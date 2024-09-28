// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DialogueComponent.h"

#include "Components/GameplayTagComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "SaveData/EventNodeSaveData.h"
#include "SaveData/PlayerOptionsSaveData.h"
#include "SaveData/SimpleDialogueSaveData.h"
#include "SaveData/TagBranchSaveData.h"


// Sets default values for this component's properties
UDialogueComponent::UDialogueComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
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

	if(!CurrentNode->NodeSaveData)
		return;
	
	if(CurrentNode->NodeSaveData->GetClass() == USimpleDialogueSaveData::StaticClass())
	{
		USimpleDialogueSaveData* saveData = Cast<USimpleDialogueSaveData>(CurrentNode->NodeSaveData);
		DialogueWidget->OnNewDialogue(saveData->CastMember, saveData->DialogueText, saveData->DialogueSound);
	}
	
	else if(CurrentNode->NodeSaveData->GetClass() == UPlayerOptionsSaveData::StaticClass())
	{
		HandlePlayerOptions();
	}
	
	else if(CurrentNode->NodeSaveData->GetClass() == UTagBranchSaveData::StaticClass())
	{
		HandleTagBranch();
	}

	else if(CurrentNode->NodeSaveData->GetClass() == UEventNodeSaveData::StaticClass())
	{
		HandleEventNode();
	}
}

void UDialogueComponent::HandlePlayerOptions()
{
	UPlayerOptionsSaveData* saveData = Cast<UPlayerOptionsSaveData>(CurrentNode->NodeSaveData);
	TMap<int, FPlayerOptions> optionsToSend;
	for (int i = 0; i <  saveData->PlayerOptions.Num(); i++)
	{
		if(saveData->PlayerOptions[i].Modifier)
		{
			if(!saveData->PlayerOptions[i].Modifier->CheckIfOptionIsAvailable())
				continue;
		}
		optionsToSend.Add(i,saveData->PlayerOptions[i]);
	}
	DialogueWidget->OnNewOptions(optionsToSend);
}

void UDialogueComponent::HandleTagBranch()
{
	UTagBranchSaveData* saveData = Cast<UTagBranchSaveData>(CurrentNode->NodeSaveData);
	ACharacter* playerCharacter =  UGameplayStatics::GetPlayerCharacter(GetWorld(),0);

	UGameplayTagComponent* tagComponent = Cast<UGameplayTagComponent>(playerCharacter->GetComponentByClass(UGameplayTagComponent::StaticClass()));

	if(!tagComponent)
	{
		OnNextDialogue(1);
		return;
	}

	// If player doesn't have all the necessary tags
	if(!tagComponent->Tags.HasAll(saveData->NecessaryTags))
	{
		OnNextDialogue(1);
		return;
	}
	
	// If player has an excluded tag
	if(tagComponent->Tags.HasAny(saveData->ExcludedTags))
	{
		OnNextDialogue(1);
		return;
	}

	//Success
	OnNextDialogue(0);
}

void UDialogueComponent::HandleEventNode()
{
	UEventNodeSaveData* saveData = Cast<UEventNodeSaveData>(CurrentNode->NodeSaveData);
	if(!Dialogue->CallOnDelegateByName(saveData->EventName))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Not bound"));
	}
	OnNextDialogue(0);
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
	DialogueWidget->OnDialogueOver();
	DialogueWidget->OnNextDialogue.RemoveDynamic(this, &UDialogueComponent::OnNextDialogue);
	DialogueWidget->RemoveFromParent();
	DialogueWidget = nullptr;
	OnDialougeTreeCompleted.Broadcast();
}

