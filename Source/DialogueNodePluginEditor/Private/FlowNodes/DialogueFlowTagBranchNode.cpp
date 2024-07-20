// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\FlowNodes\DialogueFlowTagBranchNode.h"

#include "NativeGameplayTags.h"
#include "SGraphPin.h"
#include "SaveData/TagBranchSaveData.h"

FText UDialogueFlowTagBranchNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return INVTEXT("Tag Branch");
}

FLinearColor UDialogueFlowTagBranchNode::GetNodeTitleColor() const
{
	return FColor::White;
}

void UDialogueFlowTagBranchNode::SetSaveData(UDialogueNodeSaveData* InData)
{
	if(!InData)
	{
		InData = NewObject<UTagBranchSaveData>(this, UTagBranchSaveData::StaticClass());
	}
	
	Super::SetSaveData(InData);

	Data->OnPropertyChanged.AddUObject(this, &UDialogueFlowTagBranchNode::OnPropertiesChanged);
}

TSharedPtr<SGraphNode> UDialogueFlowTagBranchNode::CreateVisualWidget()
{
	return SAssignNew(GraphNodeWidget, STagBranchNode, this);
}

void UDialogueFlowTagBranchNode::OnPropertiesChanged()
{
	if(GraphNodeWidget.IsValid())
		GraphNodeWidget->UpdateGraphNode();
}

// -------------------- Style ---------------------

void STagBranchNode::Construct(const FArguments& InArgs, UDialogueFlowTagBranchNode* InNode)
{
	GraphNode = InNode;

	TagBranchNode = InNode;
	
	SetCursor(EMouseCursor::CardinalCross);
	UpdateGraphNode();
}

TSharedRef<SWidget> STagBranchNode::CreateNodeContentArea()
{
	const FString FilterString;
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
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0, 7, 0 ,0)
				[
					SNew(STextBlock)
					.Text(INVTEXT("Necessary Tags:"))
				]
				+SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0, 4, 0 ,0)
				[
				SAssignNew(NecessaryTagCombo, SGameplayTagContainerCombo)
					.Visibility( EVisibility::Visible)
					.Filter(FilterString)
					.TagContainer(this, &STagBranchNode::GetNecessaryTagContainer)
					.OnTagContainerChanged(this, &STagBranchNode::OnNecessaryTagContainerChanged)
				]
				+SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0, 10, 0 ,0)
				[
					SNew(STextBlock)
					.Text(INVTEXT("Excluded Tags:"))
				]
				+SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0, 4, 0 ,7)
				[
				SAssignNew(ExcludedTagCombo, SGameplayTagContainerCombo)
					.Visibility( EVisibility::Visible)
					.Filter(FilterString)
					.TagContainer(this, &STagBranchNode::GetExcludedTagContainer)
					.OnTagContainerChanged(this, &STagBranchNode::OnExcludedTagContainerChanged)
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

void STagBranchNode::UpdateGraphNode()
{
	SGraphNode::UpdateGraphNode();
}

void STagBranchNode::OnNecessaryTagContainerChanged(const FGameplayTagContainer& NewTagContainer)
{
	if(!TagBranchNode)
		return;

	TObjectPtr<UTagBranchSaveData> saveData = Cast<UTagBranchSaveData>(TagBranchNode->Data);

	if(!saveData)
		return;

	saveData->NecessaryTags = NewTagContainer;
	TagBranchNode->Modify();
}

void STagBranchNode::OnExcludedTagContainerChanged(const FGameplayTagContainer& NewTagContainer)
{
	if(!TagBranchNode)
		return;

	TObjectPtr<UTagBranchSaveData> saveData = Cast<UTagBranchSaveData>(TagBranchNode->Data);

	if(!saveData)
		return;

	saveData->ExcludedTags = NewTagContainer;
	TagBranchNode->Modify();
}

FGameplayTagContainer STagBranchNode::GetNecessaryTagContainer() const
{
	if(!TagBranchNode)
		return FGameplayTagContainer {};

	TObjectPtr<UTagBranchSaveData> saveData = Cast<UTagBranchSaveData>(TagBranchNode->Data);

	if(!saveData)
		return FGameplayTagContainer {};

	return saveData->NecessaryTags;
}

FGameplayTagContainer STagBranchNode::GetExcludedTagContainer() const
{
	if(!TagBranchNode)
		return FGameplayTagContainer {};

	TObjectPtr<UTagBranchSaveData> saveData = Cast<UTagBranchSaveData>(TagBranchNode->Data);

	if(!saveData)
		return FGameplayTagContainer {};

	return saveData->ExcludedTags;
}
