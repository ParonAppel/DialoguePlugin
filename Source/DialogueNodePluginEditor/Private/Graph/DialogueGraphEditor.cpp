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
	DialogueGraph->Modify();
	DialogueGraph->NotifyGraphChanged();
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

	FString ExportedText;

	FEdGraphUtilities::ExportNodesToText(SelectedNodes, ExportedText);
	FPlatformApplicationMisc::ClipboardCopy(*ExportedText);
	
}

void SDialogueGraphEditor::PasteNodes()
{
	const FScopedTransaction Transaction(INVTEXT("Paste Node"));
	DialogueGraph->Modify();

	UDialogueGraphNode* SelectedParent = nullptr;
	bool bHasMultipleNodesSelected = false;

	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		UDialogueGraphNode* Node = Cast<UDialogueGraphNode>(*SelectedIter);

		if (Node)
		{
			if (SelectedParent == nullptr)
			{
				SelectedParent = Node;
			}
			else
			{
				bHasMultipleNodesSelected = true;

				break;
			}
		}
	}

	// Clear the selection set (newly pasted stuff will be selected)
	ClearSelectionSet();

	// Grab the text to paste from the clipboard.
	FString TextToImport;
	FPlatformApplicationMisc::ClipboardPaste(TextToImport);

	// Import the nodes
	TSet<UEdGraphNode*> PastedNodes;
	FEdGraphUtilities::ImportNodesFromText(DialogueGraph, TextToImport, /*out*/ PastedNodes);

	//Average position of nodes so we can move them while still maintaining relative distances to each other
	FVector2D AvgNodePosition(0.0f, 0.0f);
	
	// Number of nodes used to calculate AvgNodePosition
	int32 AvgCount = 0;
	
	for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
	{
		UEdGraphNode* EdNode = *It;
		UDialogueGraphNode* DialogueGraphNode = Cast<UDialogueGraphNode>(EdNode);
		if (EdNode && DialogueGraphNode)
		{
			AvgNodePosition.X += EdNode->NodePosX;
			AvgNodePosition.Y += EdNode->NodePosY;
			++AvgCount;
		}
	}
	
	if (AvgCount > 0)
	{
		float InvNumNodes = 1.0f / float(AvgCount);
		AvgNodePosition.X *= InvNumNodes;
		AvgNodePosition.Y *= InvNumNodes;
	}

	FVector2d location = GetPasteLocation();
	
	for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
	{
		UEdGraphNode* PasteNode = *It;
		UDialogueGraphNode* PasteFlowGraphNode = Cast<UDialogueGraphNode>(PasteNode);

		if (PasteFlowGraphNode)
		{
			// Select the newly pasted stuff
			constexpr bool bSelectNodes = true;
			SetNodeSelection(PasteNode, bSelectNodes);

			PasteNode->NodePosX = (PasteNode->NodePosX - AvgNodePosition.X) + location.X;
			PasteNode->NodePosY = (PasteNode->NodePosY - AvgNodePosition.Y) + location.Y;

			PasteNode->SnapToGrid(16);

			// Give new node a different Guid from the old one
			PasteNode->CreateNewGuid();
		}
	}
	
	// Update UI
	NotifyGraphChanged();

	UObject* GraphOwner = DialogueGraph->GetOuter();
	if (GraphOwner)
	{
		GraphOwner->PostEditChange();
		GraphOwner->MarkPackageDirty();
	}
	
}
