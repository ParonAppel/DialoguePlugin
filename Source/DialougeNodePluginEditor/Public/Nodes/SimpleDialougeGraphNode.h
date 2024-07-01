// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialougeGraphNode.h"
#include "SGraphNode.h"
#include "SaveData/SimpleDialougeSaveData.h"
#include "SimpleDialougeGraphNode.generated.h"

/**
 * 
 */
UCLASS()
class DIALOUGENODEPLUGINEDITOR_API USimpleDialougeGraphNode : public UDialougeGraphNode
{
	GENERATED_BODY()

public:

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual bool CanUserDeleteNode() const override { return true; }
	virtual void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;

	virtual void SetSaveData(UDialougeNodeSaveData* InData) override;

	virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;

private:

	UFUNCTION()
	void OnPropertiesChanged();
	
	TSharedPtr<SGraphNode> GraphNode = nullptr;
	
};

class DIALOUGENODEPLUGINEDITOR_API SSimpleDialougeGraphNode : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SSimpleDialougeGraphNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, USimpleDialougeGraphNode* InNode);

	virtual TSharedRef<SWidget> CreateNodeContentArea() override;

	virtual void UpdateGraphNode() override;

protected:
	USimpleDialougeGraphNode* SimpleDialougeGraphNode = nullptr;
	
	TSharedPtr<STextBlock> DialougeTextBlock = nullptr;
};