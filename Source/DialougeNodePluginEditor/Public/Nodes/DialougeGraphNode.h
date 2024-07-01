// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "DialougeGraphNode.generated.h"

class UDialougeNodeSaveData;
/**
 * 
 */
UCLASS()
class DIALOUGENODEPLUGINEDITOR_API UDialougeGraphNode : public UEdGraphNode
{
	GENERATED_BODY()
	
public:
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual bool CanUserDeleteNode() const override { return true; }
	virtual void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;
	virtual void SetSaveData(UDialougeNodeSaveData* InData);

	UEdGraphPin* CreateCustomPin(EEdGraphPinDirection Direction, FName Name);

	UPROPERTY()
	UDialougeNodeSaveData* Data = nullptr;
};

