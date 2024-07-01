// Fill out your copyright notice in the Description page of Project Settings.


#include "Nodes/SimpleDialougeGraphNode.h"

#include "Graph/DialougeGraph.h"


FText USimpleDialougeGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if(Data)
	{
		USimpleDialougeSaveData* data = Cast<USimpleDialougeSaveData>(Data);
		return data->CastMember.Name;
	}
	return Super::GetNodeTitle(TitleType);
}

FLinearColor USimpleDialougeGraphNode::GetNodeTitleColor() const
{
	if(Data)
	{
		USimpleDialougeSaveData* data = Cast<USimpleDialougeSaveData>(Data);
		return data->CastMember.Color;
	}
	return Super::GetNodeTitleColor();
}

void USimpleDialougeGraphNode::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	Super::GetNodeContextMenuActions(Menu, Context);
}

void USimpleDialougeGraphNode::SetSaveData(UDialougeNodeSaveData* InData)
{
	if(!InData)
	{
		InData = NewObject<USimpleDialougeSaveData>(this, USimpleDialougeSaveData::StaticClass());
	}
	
	Super::SetSaveData(InData);

	Data->OnPropertyChanged.AddUObject(this, &USimpleDialougeGraphNode::OnPropertiesChanged);
}



TSharedPtr<SGraphNode> USimpleDialougeGraphNode::CreateVisualWidget()
{
	return SAssignNew(GraphNode, SSimpleDialougeGraphNode, this);
}

void USimpleDialougeGraphNode::OnPropertiesChanged()
{
	if(GraphNode.IsValid())
		GraphNode->UpdateGraphNode();
}


// -------------------- Style ---------------------

void SSimpleDialougeGraphNode::Construct(const FArguments& InArgs, USimpleDialougeGraphNode* InNode)
{
	GraphNode = InNode;

	SimpleDialougeGraphNode = InNode;

	SetCursor(EMouseCursor::CardinalCross);
	UpdateGraphNode();
	
}

TSharedRef<SWidget> SSimpleDialougeGraphNode::CreateNodeContentArea()
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
				SAssignNew(DialougeTextBlock, STextBlock)
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.HAlign(HAlign_Right)
			[
				SAssignNew(RightNodeBox, SVerticalBox)
			]
		];	
}

void SSimpleDialougeGraphNode::UpdateGraphNode()
{
	SGraphNode::UpdateGraphNode();

	if(DialougeTextBlock)
	{
		USimpleDialougeSaveData* data = Cast<USimpleDialougeSaveData>(SimpleDialougeGraphNode->Data);
		DialougeTextBlock->SetText(data->DialougeText);
	}
}

