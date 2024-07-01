// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/DialougeGraphSchema.h"

#include "Nodes/DialougeGraphNode.h"
#include "Nodes/SimpleDialougeGraphNode.h"
#include "Nodes/StartDialougeGraphNode.h"


void UDialougeGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	Super::GetGraphContextActions(ContextMenuBuilder);

	TSharedPtr<FNewNodeAction> NewNodeAction (
		new FNewNodeAction(
			INVTEXT("Nodes"),
			INVTEXT("New Simple Dialouge Node"),
			INVTEXT("Makes A New Simple Dialouge Node"),
			0
			)
		);

	ContextMenuBuilder.AddAction(NewNodeAction);
}

const FPinConnectionResponse UDialougeGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{

	if(!A || !B)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("nullptrs"));
	}

	if(A->Direction == B->Direction)
	{	
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("inputs need to go to outputs"));
	}
	
	return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_AB, TEXT(""));
}

void UDialougeGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	CreateStartNode(Graph);
	Super::CreateDefaultNodesForGraph(Graph);
}

void UDialougeGraphSchema::CreateStartNode(UEdGraph& Graph) const
{
	UStartDialougeGraphNode* result = NewObject<UStartDialougeGraphNode>(&Graph);
	result->CreateNewGuid();
	result->NodePosX = 0;
	result->NodePosY = 0;
	
	result->SetSaveData(nullptr);
	
	result->CreateCustomPin(EGPD_Output, TEXT(""));
	
	
	Graph.Modify();
	Graph.AddNode(result, true, true);
}

UEdGraphNode* FNewNodeAction::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location,
                                            bool bSelectNewNode)
{
	USimpleDialougeGraphNode* result = NewObject<USimpleDialougeGraphNode>(ParentGraph);
	result->CreateNewGuid();
	result->NodePosX = Location.X;
	result->NodePosY = Location.Y;
	
	result->SetSaveData(nullptr);
	
	UEdGraphPin* inputPin = result->CreateCustomPin(EGPD_Input, TEXT(""));
	result->CreateCustomPin(EGPD_Output, TEXT(""));

	if(FromPin)
	{
		result->GetSchema()->TryCreateConnection(FromPin, inputPin);
	}
	
	ParentGraph->Modify();
	ParentGraph->AddNode(result, true, true);
	return result;
}
