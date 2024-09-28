// Fill out your copyright notice in the Description page of Project Settings.


#include "Nodes/EventGraphNode.h"
#include "SaveData/EventNodeSaveData.h"
#include "Graph/DialogueGraph.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"


FText UEventGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return INVTEXT("Event Node");
}

FLinearColor UEventGraphNode::GetNodeTitleColor() const
{
	return FColor::White;
}

void UEventGraphNode::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	Super::GetNodeContextMenuActions(Menu, Context);
}

FSlateIcon UEventGraphNode::GetIconAndTint(FLinearColor& OutColor) const
{
	return FSlateIcon(TEXT("EditorStyle"), TEXT("Icons.JumpToEvent"));
}

void UEventGraphNode::SetSaveData(UDialogueNodeSaveData* InData)
{
	if(!InData)
	{
		InData = NewObject<UEventNodeSaveData>(this, UEventNodeSaveData::StaticClass());
	}
	
	Super::SetSaveData(InData);

	Data->OnPropertyChanged.AddUObject(this, &UEventGraphNode::OnPropertiesChanged);
}



TSharedPtr<SGraphNode> UEventGraphNode::CreateVisualWidget()
{
	return SAssignNew(GraphNodeWidget, SEventGraphNode, this);
}

void UEventGraphNode::OnPropertiesChanged()
{
	if(GraphNodeWidget.IsValid())
		GraphNodeWidget->UpdateGraphNode();
}


// -------------------- Style ---------------------

void SEventGraphNode::Construct(const FArguments& InArgs, UEventGraphNode* InNode)
{
	GraphNode = InNode;

	SimpleDialogueGraphNode = InNode;
	
	SetCursor(EMouseCursor::CardinalCross);
	UpdateGraphNode();
	
}

TSharedRef<SWidget> SEventGraphNode::CreateNodeContentArea()
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
					.OnTextCommitted(this, &SEventGraphNode::OnTextCommitted)
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

void SEventGraphNode::UpdateGraphNode()
{
	SGraphNode::UpdateGraphNode();
}

void SEventGraphNode::OnTextCommitted(const FText& NewText, ETextCommit::Type CommitType)
{
}

