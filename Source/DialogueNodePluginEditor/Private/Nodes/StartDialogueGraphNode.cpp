// Fill out your copyright notice in the Description page of Project Settings.


#include "Nodes/StartDialogueGraphNode.h"

FText UStartDialogueGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return INVTEXT("Start");
}

FLinearColor UStartDialogueGraphNode::GetNodeTitleColor() const
{
	return FLinearColor::Red;
}
