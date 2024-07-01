// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialougeGraphNode.h"
#include "StartDialougeGraphNode.generated.h"

/**
 * 
 */
UCLASS()
class DIALOUGENODEPLUGINEDITOR_API UStartDialougeGraphNode : public UDialougeGraphNode
{
	GENERATED_BODY()
	
public:
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual bool CanUserDeleteNode() const override { return false; }
	
};
