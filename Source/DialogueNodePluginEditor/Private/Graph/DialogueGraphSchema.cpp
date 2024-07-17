// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/DialogueGraphSchema.h"

#include "Nodes/DialogueGraphNode.h"
#include "Nodes/PlayerOptionsGraphNode.h"
#include "Nodes/SimpleDialogueGraphNode.h"
#include "Nodes/StartDialogueGraphNode.h"


void UDialogueGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	Super::GetGraphContextActions(ContextMenuBuilder);

	TSharedPtr<FNewSimpleDialogueNodeAction> NewSimpleDialogueNodeAction (
		new FNewSimpleDialogueNodeAction(
			INVTEXT("Nodes"),
			INVTEXT("New Simple Dialogue Node"),
			INVTEXT("Makes A New Simple Dialogue Node"),
			0
			)
		);

	ContextMenuBuilder.AddAction(NewSimpleDialogueNodeAction);

	TSharedPtr<FNewPlayerOptionsNodeAction> NewPlayerOptionsNodeAction (
		new FNewPlayerOptionsNodeAction(
			INVTEXT("Nodes"),
			INVTEXT("New Player Option Node"),
			INVTEXT("Makes A New Player Option Node"),
			0
			)
		);

	ContextMenuBuilder.AddAction(NewPlayerOptionsNodeAction);
}

const FPinConnectionResponse UDialogueGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
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

void UDialogueGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	CreateStartNode(Graph);
	Super::CreateDefaultNodesForGraph(Graph);
}

void UDialogueGraphSchema::CreateStartNode(UEdGraph& Graph) const
{
	UStartDialogueGraphNode* result = NewObject<UStartDialogueGraphNode>(&Graph);
	result->CreateNewGuid();
	result->NodePosX = 0;
	result->NodePosY = 0;
	
	result->SetSaveData(nullptr);
	
	result->CreateCustomPin(EGPD_Output, TEXT(""));
	
	
	Graph.Modify();
	Graph.AddNode(result, true, true);
}

UEdGraphNode* FNewSimpleDialogueNodeAction::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location,
                                            bool bSelectNewNode)
{
	USimpleDialogueGraphNode* result = NewObject<USimpleDialogueGraphNode>(ParentGraph);
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

UEdGraphNode* FNewPlayerOptionsNodeAction::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin,
	const FVector2D Location, bool bSelectNewNode)
{
	UPlayerOptionsGraphNode* result = NewObject<UPlayerOptionsGraphNode>(ParentGraph);
	result->CreateNewGuid();
	result->NodePosX = Location.X;
	result->NodePosY = Location.Y;
	
	result->SetSaveData(nullptr);
	
	UEdGraphPin* inputPin = result->CreateCustomPin(EGPD_Input, TEXT(""));

	if(FromPin)
	{
		result->GetSchema()->TryCreateConnection(FromPin, inputPin);
	}
	
	ParentGraph->Modify();
	ParentGraph->AddNode(result, true, true);
	return result;
}
