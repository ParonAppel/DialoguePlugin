// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"
#include "Nodes/DialogueGraphNode.h"
#include "SGameplayTagContainerCombo.h"
#include "DialogueFlowTagBranchNode.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGUENODEPLUGINEDITOR_API UDialogueFlowTagBranchNode : public UDialogueGraphNode
{
	GENERATED_BODY()
	
public:

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual bool CanUserDeleteNode() const override { return true; }

	virtual void SetSaveData(UDialogueNodeSaveData* InData) override;

	virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;

private:
	UFUNCTION()
	void OnPropertiesChanged();

	TSharedPtr<SGraphNode> GraphNodeWidget = nullptr;
};

class DIALOGUENODEPLUGINEDITOR_API STagBranchNode : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(STagBranchNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UDialogueFlowTagBranchNode* InNode);

	virtual TSharedRef<SWidget> CreateNodeContentArea() override;

	virtual void UpdateGraphNode() override;

protected:
	UDialogueFlowTagBranchNode* TagBranchNode = nullptr;
	
	TSharedPtr<SGameplayTagContainerCombo> NecessaryTagCombo = nullptr;
	TSharedPtr<SGameplayTagContainerCombo> ExcludedTagCombo = nullptr;
	
	void OnNecessaryTagContainerChanged(const FGameplayTagContainer& NewTagContainer);
	void OnExcludedTagContainerChanged(const FGameplayTagContainer& NewTagContainer);
	
	FGameplayTagContainer GetNecessaryTagContainer() const;
	FGameplayTagContainer GetExcludedTagContainer() const;
	
};
