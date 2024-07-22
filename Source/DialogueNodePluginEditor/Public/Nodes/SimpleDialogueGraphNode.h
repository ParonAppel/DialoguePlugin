// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueGraphNode.h"
#include "SGraphNode.h"
#include "SimpleDialogueGraphNode.generated.h"

/**
 * 
 */
class SMultiLineEditableTextBox;

UCLASS()
class DIALOGUENODEPLUGINEDITOR_API USimpleDialogueGraphNode : public UDialogueGraphNode
{
	GENERATED_BODY()

public:

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual bool CanUserDeleteNode() const override { return true; }
	virtual void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;
	virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
	virtual bool ShowPaletteIconOnNode() const override { return true; }
	virtual void SetSaveData(UDialogueNodeSaveData* InData) override;

	virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;

private:

	UFUNCTION()
	void OnPropertiesChanged();
	
	TSharedPtr<SGraphNode> GraphNodeWidget = nullptr;
	
};

class DIALOGUENODEPLUGINEDITOR_API SSimpleDialogueGraphNode : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SSimpleDialogueGraphNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, USimpleDialogueGraphNode* InNode);

	virtual TSharedRef<SWidget> CreateNodeContentArea() override;

	virtual void UpdateGraphNode() override;

	void OnTextCommitted( const FText& NewText, ETextCommit::Type CommitType);

protected:
	USimpleDialogueGraphNode* SimpleDialogueGraphNode = nullptr;
	
	TSharedPtr<SMultiLineEditableTextBox> DialogueTextBlock = nullptr;
};