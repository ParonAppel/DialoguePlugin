// Fill out your copyright notice in the Description page of Project Settings.


#include "Nodes/SimpleDialogueGraphNode.h"
#include "SaveData/SimpleDialogueSaveData.h"
#include "Graph/DialogueGraph.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"


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

FSlateIcon USimpleDialogueGraphNode::GetIconAndTint(FLinearColor& OutColor) const
{
	return FSlateIcon(TEXT("EditorStyle"), TEXT("MainFrame.VisitForums"));
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
	return SAssignNew(GraphNodeWidget, SSimpleDialogueGraphNode, this);
}

void USimpleDialogueGraphNode::OnPropertiesChanged()
{
	if(GraphNodeWidget.IsValid())
		GraphNodeWidget->UpdateGraphNode();
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
			.Padding(0,7,0,7)
			.AutoWidth()
			.HAlign(HAlign_Fill)
			[
				SNew(SBox)
				.MaxDesiredWidth(300)
				[
					SAssignNew(DialogueTextBlock, SMultiLineEditableTextBox)
					.IsReadOnly(false)
					.OnTextCommitted(this, &SSimpleDialogueGraphNode::OnTextCommitted)
					.WrapTextAt(300)
					.AutoWrapText(true)
					.WrappingPolicy(ETextWrappingPolicy::DefaultWrapping)
				]
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

void SSimpleDialogueGraphNode::OnTextCommitted(const FText& NewText, ETextCommit::Type CommitType)
{
	USimpleDialogueSaveData* data = Cast<USimpleDialogueSaveData>(SimpleDialogueGraphNode->Data);
	data->DialogueText = NewText;
}

