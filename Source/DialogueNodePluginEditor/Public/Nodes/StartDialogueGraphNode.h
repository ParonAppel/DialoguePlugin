// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueGraphNode.h"
#include "StartDialogueGraphNode.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGUENODEPLUGINEDITOR_API UStartDialogueGraphNode : public UDialogueGraphNode
{
	GENERATED_BODY()
	
public:
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual bool CanUserDeleteNode() const override { return false; }
	virtual bool CanDuplicateNode() const override { return false;}
	
};
