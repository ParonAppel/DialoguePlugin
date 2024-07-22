// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueGraphNode.h"
#include "PlayerOptionsGraphNode.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGUENODEPLUGINEDITOR_API UPlayerOptionsGraphNode : public UDialogueGraphNode
{
	GENERATED_BODY()
	
public:
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual bool CanUserDeleteNode() const override { return true; }
	virtual bool ShowPaletteIconOnNode() const override { return true; }
	virtual void SetSaveData(UDialogueNodeSaveData* InData) override;

	virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;
	
	virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;

private:

	TSharedPtr<SGraphNode> GraphNodeWidget = nullptr;

	UFUNCTION()
	void OnPropertiesChanged();
};
