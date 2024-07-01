// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/DialougeGraphEditor.h"

#include "EdGraphUtilities.h"
#include "EditorAnalytics.h"
#include "GraphEditorActions.h"
#include "Framework/Commands/GenericCommands.h"
#include "Nodes/DialougeGraphNode.h"
#include "GraphEditor.h"
#include "Windows/WindowsPlatformApplicationMisc.h"


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

bool SDialougeGraphEditor::CanDelete()
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

bool SDialougeGraphEditor::CanCopyNodes()
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

bool SDialougeGraphEditor::CanPasteNodes()
{
	if (!CanEdit())
	{
		return false;
	}

	FString ClipboardContent;
	FPlatformApplicationMisc::ClipboardPaste(ClipboardContent);

	const bool bIsPastePossible = FEdGraphUtilities::CanImportNodesFromText(DialougeGraph, ClipboardContent);

	return bIsPastePossible;
}

void SDialougeGraphEditor::BindGraphCommands()
{

	FGraphEditorCommands::Register();
	
	const FGenericCommands& GenericCommands = FGenericCommands::Get();
	const FGraphEditorCommandsImpl& GraphEditorCommands = FGraphEditorCommands::Get();
	
	CommandList = MakeShareable(new FUICommandList);

	//Generic Commands
	CommandList->MapAction(GenericCommands.Undo,
								   FExecuteAction::CreateRaw(this, &SDialougeGraphEditor::UndoGraphAction),
								   FCanExecuteAction::CreateRaw(this, &SDialougeGraphEditor::CanEdit));

	CommandList->MapAction(GenericCommands.Delete,
								   FExecuteAction::CreateRaw(this, &SDialougeGraphEditor::DeleteSelectedNodes),
								   FCanExecuteAction::CreateRaw(this, &SDialougeGraphEditor::CanDelete));

	CommandList->MapAction(GenericCommands.Copy,
								   FExecuteAction::CreateRaw(this, &SDialougeGraphEditor::CopySelectedNodes),
								   FCanExecuteAction::CreateRaw(this, &SDialougeGraphEditor::CanCopyNodes));

	CommandList->MapAction(GenericCommands.Paste,
								   FExecuteAction::CreateRaw(this, &SDialougeGraphEditor::PasteNodes),
								   FCanExecuteAction::CreateRaw(this, &SDialougeGraphEditor::CanPasteNodes));
	
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
	GetCurrentGraph()->Modify();
}

void SDialougeGraphEditor::DeleteSelectedNodes()
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

void SDialougeGraphEditor::CopySelectedNodes()
{
	// Export the selected nodes and place the text on the clipboard
	FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	FGraphPanelSelectionSet NewSelectedNodes;

	FString ExportedText;

	FEdGraphUtilities::ExportNodesToText(NewSelectedNodes, ExportedText);
	FPlatformApplicationMisc::ClipboardCopy(*ExportedText);
	
}

void SDialougeGraphEditor::PasteNodes()
{
}
