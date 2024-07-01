// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/DialougeGraphEditor.h"

#include "EditorAnalytics.h"
#include "GraphEditorActions.h"
#include "Framework/Commands/GenericCommands.h"
#include "Nodes/DialougeGraphNode.h"


void SDialougeGraphEditor::Construct(const FArguments& InArgs, const TObjectPtr<UDialougeGraph> InEditor)
{
	BindGraphCommands();

	DetailsView = InArgs._DetailsView;
	DialougeGraph = InEditor;
	
	SGraphEditor::FArguments Arguments;
	Arguments._AdditionalCommands = CommandList;
	Arguments._GraphToEdit = Cast<UEdGraph>(DialougeGraph.Get());
	Arguments._GraphEvents.OnSelectionChanged = FOnSelectionChanged::CreateSP(this, &SDialougeGraphEditor::OnSelectedNodesChanged);
	
	SGraphEditor::Construct(Arguments);
	
}

bool SDialougeGraphEditor::CanEdit()
{
	return GEditor->PlayWorld == nullptr;
}

void SDialougeGraphEditor::BindGraphCommands()
{

	FGraphEditorCommands::Register();
	
	const FGenericCommands& GenericCommands = FGenericCommands::Get();
	const FGraphEditorCommandsImpl& GraphEditorCommands = FGraphEditorCommands::Get();
	
	CommandList = MakeShareable(new FUICommandList);

	//Generic Commands
	
	CommandList->MapAction(GenericCommands.Undo,
								   FExecuteAction::CreateStatic(&SDialougeGraphEditor::UndoGraphAction),
								   FCanExecuteAction::CreateStatic(&SDialougeGraphEditor::CanEdit));

	
}

void SDialougeGraphEditor::OnSelectedNodesChanged(const TSet<UObject*>& Nodes)
{
	TArray<UObject*> SelectedObjects;

	if (Nodes.Num() > 0)
	{
		for (TSet<UObject*>::TConstIterator SetIt(Nodes); SetIt; ++SetIt)
		{
			if (const UDialougeGraphNode* GraphNode = Cast<UDialougeGraphNode>(*SetIt))
			{
				SelectedObjects.Add(Cast<UObject>(GraphNode->Data));
			}
			else
			{
				SelectedObjects.Add(*SetIt);
			}
		}
	}
	else
	{
		SelectedObjects.Add(Cast<UObject>(DialougeGraph->DialougeAsset));
	}

	if(DetailsView.IsValid())
		DetailsView->SetObjects(SelectedObjects);
	
	OnSelectionChangedEvent.ExecuteIfBound(Nodes);
}

void SDialougeGraphEditor::UndoGraphAction()
{
	GEditor->UndoTransaction();
}
