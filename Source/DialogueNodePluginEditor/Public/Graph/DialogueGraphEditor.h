// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueGraph.h"

/**
 * 
 */
class DIALOGUENODEPLUGINEDITOR_API SDialogueGraphEditor : public SGraphEditor
{
public:
	SLATE_BEGIN_ARGS(SDialogueGraphEditor)
		{
		}
		
		SLATE_ARGUMENT(TSharedPtr<IDetailsView>, DetailsView)
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs, const TObjectPtr<UDialogueGraph> InEditor);
	
	bool CanEdit();
	bool CanDelete();
	bool CanCopyNodes();
	bool CanPasteNodes();

	FOnSelectionChanged OnSelectionChangedEvent;

protected:

	void BindGraphCommands();
	virtual void OnSelectedNodesChanged(const TSet<UObject*>& Nodes);
	void UndoGraphAction();
	void DeleteSelectedNodes();
	void CopySelectedNodes();
	void PasteNodes();
	
	TSharedPtr<FUICommandList> CommandList = nullptr;
	TSharedPtr<IDetailsView> DetailsView = nullptr;
	TObjectPtr<UDialogueGraph> DialogueGraph = nullptr;
};
