// Fill out your copyright notice in the Description page of Project Settings.


#include "Nodes/SimpleDialogueGraphNode.h"

#include "Graph/DialogueGraph.h"


FText USimpleDialogueGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if(Data)
	{
		USimpleDialogueSaveData* data = Cast<USimpleDialogueSaveData>(Data);
		return data->CastMember.Name;
	}
	return Super::GetNodeTitle(TitleType);
}

FLinearColor USimpleDialogueGraphNode::GetNodeTitleColor() const
{
	if(Data)
	{
		USimpleDialogueSaveData* data = Cast<USimpleDialogueSaveData>(Data);
		return data->CastMember.Color;
	}
	return Super::GetNodeTitleColor();
}

void USimpleDialogueGraphNode::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	Super::GetNodeContextMenuActions(Menu, Context);
}

void USimpleDialogueGraphNode::SetSaveData(UDialogueNodeSaveData* InData)
{
	if(!InData)
	{
		InData = NewObject<USimpleDialogueSaveData>(this, USimpleDialogueSaveData::StaticClass());
	}
	
	Super::SetSaveData(InData);

	Data->OnPropertyChanged.AddUObject(this, &USimpleDialogueGraphNode::OnPropertiesChanged);
}



TSharedPtr<SGraphNode> USimpleDialogueGraphNode::CreateVisualWidget()
{
	return SAssignNew(GraphNode, SSimpleDialogueGraphNode, this);
}

void USimpleDialogueGraphNode::OnPropertiesChanged()
{
	if(GraphNode.IsValid())
		GraphNode->UpdateGraphNode();
}


// -------------------- Style ---------------------

void SSimpleDialogueGraphNode::Construct(const FArguments& InArgs, USimpleDialogueGraphNode* InNode)
{
	GraphNode = InNode;

	SimpleDialogueGraphNode = InNode;

	SetCursor(EMouseCursor::CardinalCross);
	UpdateGraphNode();
	
}

TSharedRef<SWidget> SSimpleDialogueGraphNode::CreateNodeContentArea()
{
	return SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush("NoBorder"))
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.FillWidth(1.0f)
			[
				SAssignNew(LeftNodeBox, SVerticalBox)
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.HAlign(HAlign_Fill)
			[
				SAssignNew(DialogueTextBlock, STextBlock)
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.HAlign(HAlign_Right)
			[
				SAssignNew(RightNodeBox, SVerticalBox)
			]
		];	
}

void SSimpleDialogueGraphNode::UpdateGraphNode()
{
	SGraphNode::UpdateGraphNode();

	if(DialogueTextBlock)
	{
		USimpleDialogueSaveData* data = Cast<USimpleDialogueSaveData>(SimpleDialogueGraphNode->Data);
		DialogueTextBlock->SetText(data->DialogueText);
	}
}

