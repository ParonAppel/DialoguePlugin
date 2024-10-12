// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueGraphNode.h"
#include "SGraphNode.h"
#include "EventGraphNode.generated.h"

/**
 * 
 */
class SNameComboBox;

UCLASS()
class DIALOGUENODEPLUGINEDITOR_API UEventGraphNode : public UDialogueGraphNode
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

class DIALOGUENODEPLUGINEDITOR_API SEventGraphNode : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SEventGraphNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEventGraphNode* InNode);

	virtual TSharedRef<SWidget> CreateNodeContentArea() override;

	virtual void UpdateGraphNode() override;

	void OnSelectionChanged( TSharedPtr<FName> NameItem, ESelectInfo::Type SelectInfo);

protected:

	TArray<TSharedPtr<FName>> NameList;
	TSharedPtr<FName> CurrentlySelectedName;
	UEventGraphNode* SimpleDialogueGraphNode = nullptr;
	TSharedPtr<SNameComboBox> NameComboBox = nullptr;
};