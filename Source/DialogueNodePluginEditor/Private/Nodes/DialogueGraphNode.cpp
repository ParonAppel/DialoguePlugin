// Fill out your copyright notice in the Description page of Project Settings.


#include "Nodes/DialogueGraphNode.h"
#include "Framework/Commands/UIAction.h"
#include "DialogueNodePlugin/Public/DialogueNodeSaveData.h"
#include "ToolMenu.h"

FText UDialogueGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return INVTEXT("Node");
}

FLinearColor UDialogueGraphNode::GetNodeTitleColor() const
{
	return FColor::Green;
}

void UDialogueGraphNode::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	FToolMenuSection& toolMenuSection = Menu->AddSection(TEXT("Pins"), INVTEXT("Custom Node Actions"));
	
	UDialogueGraphNode* node = (UDialogueGraphNode*) this;
	//
	// toolMenuSection.AddMenuEntry(
	// 	TEXT("AddPinEntry"),
	// 	INVTEXT("Add Pin"),
	// 	INVTEXT("Creates A New Pin"),
	// 	FSlateIcon(TEXT("DialogueEditorStyle"), TEXT("DialogueNodePluginEditor.AddPinIcon")),
	// 	FUIAction(FExecuteAction::CreateLambda(
	// 		[node]() 
	// 		{
	// 			
	// 			node->CreateCustomPin(
	// 				EEdGraphPinDirection::EGPD_Output,
	// 				TEXT("Another Output")
	// 				);
	// 			
	// 			node->GetGraph()->NotifyGraphChanged();
	// 			node->GetGraph()->Modify();
	// 		}
	// 	))
	// );
	//
	// toolMenuSection.AddMenuEntry(
	// 	TEXT("RemovePinEntry"),
	// 	INVTEXT("Remove Pin"),
	// 	INVTEXT("Removes The Last Pin"),
	// 	FSlateIcon(TEXT("DialogueEditorStyle"), TEXT("DialogueNodePluginEditor.RemovePinIcon")),
	// 	FUIAction(FExecuteAction::CreateLambda(
	// 		[node]() 
	// 		{
	// 			for (int i = node->Pins.Num() -1; i >= 0; i--)
	// 			{
	// 				if(node->Pins[i]->Direction == EGPD_Output)
	// 				{
	// 					node->RemovePin(node->Pins[i]); 
	// 			
	// 					node->GetGraph()->NotifyGraphChanged();
	// 					node->GetGraph()->Modify();
	//
	// 					return;
	// 				}
	// 			}
	// 			
	// 		}
	// 	))
	// );
	//
	toolMenuSection.AddMenuEntry(
		TEXT("DeleteEntry"),
		INVTEXT("Delete Node"),
		INVTEXT("Deletes The Node"),
		FSlateIcon(TEXT("DialogueEditorStyle"), TEXT("DialogueNodePluginEditor.RemovePinIcon")),
		FUIAction(FExecuteAction::CreateLambda(
			[node]() 
			{
				node->GetGraph()->RemoveNode(node);
			}
		))
	);
	
}

void UDialogueGraphNode::SetSaveData(UDialogueNodeSaveData* InData)
{
	Data = InData;
}

FSlateIcon UDialogueGraphNode::GetIconAndTint(FLinearColor& OutColor) const
{
	return Super::GetIconAndTint(OutColor);
}

UEdGraphPin* UDialogueGraphNode::CreateCustomPin(EEdGraphPinDirection Direction, FName Name)
{
	FName category = (Direction == EGPD_Input) ? TEXT("Inputs") : TEXT("Outputs");
	FName subCategory = TEXT("DialoguePin");

	UEdGraphPin* pin = CreatePin(Direction, category, Name );
	pin->PinType.PinSubCategory = subCategory;

	return pin;
}

