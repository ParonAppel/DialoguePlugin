// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "DialogueGraphNode.generated.h"

class UDialogueNodeSaveData;
/**
 * 
 */
UCLASS()
class DIALOGUENODEPLUGINEDITOR_API UDialogueGraphNode : public UEdGraphNode
{
	GENERATED_BODY()
	
public:
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual bool CanUserDeleteNode() const override { return true; }
	virtual void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;
	virtual void SetSaveData(UDialogueNodeSaveData* InData);
	virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;

	UEdGraphPin* CreateCustomPin(EEdGraphPinDirection Direction, FName Name);

	UPROPERTY()
	UDialogueNodeSaveData* Data = nullptr;
};

