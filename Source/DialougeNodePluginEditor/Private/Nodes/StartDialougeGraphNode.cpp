// Fill out your copyright notice in the Description page of Project Settings.


#include "Nodes/StartDialougeGraphNode.h"

FText UStartDialougeGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return INVTEXT("Start");
}

FLinearColor UStartDialougeGraphNode::GetNodeTitleColor() const
{
	return FLinearColor::Red;
}
