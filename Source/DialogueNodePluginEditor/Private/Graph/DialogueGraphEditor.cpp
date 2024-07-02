// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/DialogueGraphEditor.h"

#include "EdGraphUtilities.h"
#include "EditorAnalytics.h"
#include "GraphEditorActions.h"
#include "Framework/Commands/GenericCommands.h"
#include "Nodes/DialogueGraphNode.h"
#include "GraphEditor.h"
#include "Windows/WindowsPlatformApplicationMisc.h"


void SDialogueGraphEditor::Construct(const FArguments& InArgs, const TObjectPtr<UDialogueGraph> InEditor)
{
	BindGraphCommands();

	DetailsView = InArgs._DetailsView;
	DialogueGraph = InEditor;
	
	SGraphEditor::FArguments Arguments;
	Arguments._AdditionalCommands = CommandList;
	Arguments._GraphToEdit = Cast<UEdGraph>(DialogueGraph.Get());
	Arguments._GraphEvents.OnSelectionChanged = FOnSelectionChanged::CreateSP(this, &SDialogueGraphEditor::OnSelectedNodesChanged);
	
	SGraphEditor::Construct(Arguments);
	
}

bool SDialogueGraphEditor::CanEdit()
{
	return GEditor->PlayWorld == nullptr;
}

bool SDialogueGraphEditor::CanDelete()
{
	if(!CanEdit())
	 return false;
	
	const FGraphPanelSelectionSet SelectedNodes = this->GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
	{
		if (const UEdGraphNode* Node = Cast<UEdGraphNode>(*NodeIt))
		{
			if (!Node->CanUserDeleteNode())
				return false;

		}
	}
	
	return SelectedNodes.Num() > 0;
}

bool SDialogueGraphEditor::CanCopyNodes()
{
	if(!CanEdit())
		return false;
	
	const FGraphPanelSelectionSet SelectedNodes = this->GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
	{
		if (const UEdGraphNode* Node = Cast<UEdGraphNode>(*NodeIt))
		{
			if (!Node->CanDuplicateNode())
				return false;

		}
	}
	
	return SelectedNodes.Num() > 0;
}

bool SDialogueGraphEditor::CanPasteNodes()
{
	if (!CanEdit())
	{
		return false;
	}

	FString ClipboardContent;
	FPlatformApplicationMisc::ClipboardPaste(ClipboardContent);

	const bool bIsPastePossible = FEdGraphUtilities::CanImportNodesFromText(DialogueGraph, ClipboardContent);

	return bIsPastePossible;
}

void SDialogueGraphEditor::BindGraphCommands()
{

	FGraphEditorCommands::Register();
	
	const FGenericCommands& GenericCommands = FGenericCommands::Get();
	const FGraphEditorCommandsImpl& GraphEditorCommands = FGraphEditorCommands::Get();
	
	CommandList = MakeShareable(new FUICommandList);

	//Generic Commands
	CommandList->MapAction(GenericCommands.Undo,
								   FExecuteAction::CreateRaw(this, &SDialogueGraphEditor::UndoGraphAction),
								   FCanExecuteAction::CreateRaw(this, &SDialogueGraphEditor::CanEdit));

	CommandList->MapAction(GenericCommands.Delete,
								   FExecuteAction::CreateRaw(this, &SDialogueGraphEditor::DeleteSelectedNodes),
								   FCanExecuteAction::CreateRaw(this, &SDialogueGraphEditor::CanDelete));

	CommandList->MapAction(GenericCommands.Copy,
								   FExecuteAction::CreateRaw(this, &SDialogueGraphEditor::CopySelectedNodes),
								   FCanExecuteAction::CreateRaw(this, &SDialogueGraphEditor::CanCopyNodes));

	CommandList->MapAction(GenericCommands.Paste,
								   FExecuteAction::CreateRaw(this, &SDialogueGraphEditor::PasteNodes),
								   FCanExecuteAction::CreateRaw(this, &SDialogueGraphEditor::CanPasteNodes));
	
}

void SDialogueGraphEditor::OnSelectedNodesChanged(const TSet<UObject*>& Nodes)
{
	TArray<UObject*> SelectedObjects;

	if (Nodes.Num() > 0)
	{
		for (TSet<UObject*>::TConstIterator SetIt(Nodes); SetIt; ++SetIt)
		{
			if (const UDialogueGraphNode* GraphNode = Cast<UDialogueGraphNode>(*SetIt))
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
		SelectedObjects.Add(Cast<UObject>(DialogueGraph->DialogueAsset));
	}

	if(DetailsView.IsValid())
		DetailsView->SetObjects(SelectedObjects);
	
	OnSelectionChangedEvent.ExecuteIfBound(Nodes);
}

void SDialogueGraphEditor::UndoGraphAction()
{
	GEditor->UndoTransaction();
	GetCurrentGraph()->Modify();
}

void SDialogueGraphEditor::DeleteSelectedNodes()
{
	const FScopedTransaction Transaction(INVTEXT("DeleteSelectedNode"));

	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();;

	for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
	{
		UEdGraphNode* Node = CastChecked<UEdGraphNode>(*NodeIt);
		if (Node->CanUserDeleteNode())
		{
			GetCurrentGraph()->GetSchema()->BreakNodeLinks(*Node);
			Node->DestroyNode();
		}
	}
	
	GetCurrentGraph()->Modify();
}

void SDialogueGraphEditor::CopySelectedNodes()
{
	// Export the selected nodes and place the text on the clipboard
	FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	FGraphPanelSelectionSet NewSelectedNodes;

	FString ExportedText;

	FEdGraphUtilities::ExportNodesToText(NewSelectedNodes, ExportedText);
	FPlatformApplicationMisc::ClipboardCopy(*ExportedText);
	
}

void SDialogueGraphEditor::PasteNodes()
{
}
