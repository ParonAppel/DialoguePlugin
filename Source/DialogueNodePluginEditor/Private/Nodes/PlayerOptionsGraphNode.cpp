// Fill out your copyright notice in the Description page of Project Settings.


#include "Nodes/PlayerOptionsGraphNode.h"
#include "EdGraph/EdGraphPin.h"
#include "SGraphNode.h"
#include "SaveData/PlayerOptionsSaveData.h"

FText UPlayerOptionsGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return INVTEXT("PlayerOptions");
}

FLinearColor UPlayerOptionsGraphNode::GetNodeTitleColor() const
{
	return FColor::White;
}

void UPlayerOptionsGraphNode::SetSaveData(UDialogueNodeSaveData* InData)
{
	if(!InData)
	{
		InData = NewObject<UPlayerOptionsSaveData>(this, UPlayerOptionsSaveData::StaticClass());
	}
	
	Super::SetSaveData(InData);

	Data->OnPropertyChanged.AddUObject(this, &UPlayerOptionsGraphNode::OnPropertiesChanged);
}

TSharedPtr<SGraphNode> UPlayerOptionsGraphNode::CreateVisualWidget()
{
	GraphNodeWidget = Super::CreateVisualWidget();
	return GraphNodeWidget;
}

FSlateIcon UPlayerOptionsGraphNode::GetIconAndTint(FLinearColor& OutColor) const
{
	return FSlateIcon( "EditorStyle", "MainFrame.VisitSearchForAnswersPage" );
}

void UPlayerOptionsGraphNode::OnPropertiesChanged()
{
	if(!Data)
		return;

	UPlayerOptionsSaveData* optionsData  = Cast<UPlayerOptionsSaveData>(Data);

	if(!optionsData)
		return;

	TArray<FString> playerOptions;
	TMap<FString, UEdGraphPin*> pinsAndTheirNames;
	
	for(UEdGraphPin* pin : GetAllPins())
	{
		if(pin->Direction == EGPD_Input)
		{
			continue;
		}
		pinsAndTheirNames.Add(pin->GetName(), pin);	
	}
	
	for ( FPlayerOptions playerOption : optionsData->PlayerOptions)
	{
		UEdGraphPin** pin = pinsAndTheirNames.Find(playerOption.OptionsText.ToString());
		if(!pin)
		{
			CreateCustomPin(EGPD_Output, FName(playerOption.OptionsText.ToString()));
		}
		playerOptions.Add(playerOption.OptionsText.ToString());	
	}

	TArray<FString> pins;
	pinsAndTheirNames.GetKeys(pins);
	
	for (FString pinName : pins) 
	{
		if(playerOptions.Find(pinName) == -1)
		{
			RemovePin(*pinsAndTheirNames.Find(pinName));
			pinsAndTheirNames.Remove(pinName);
		}
	}

	this->GetGraph()->NotifyNodeChanged(this);
	this->GetGraph()->Modify();
}
