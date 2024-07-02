// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Graph/DialogueGraph.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "DialogueNodePlugin/Public/Dialogue.h"

class SDialogueGraphEditor;
/**
 * 
 */
class DIALOGUENODEPLUGINEDITOR_API FDialogueEditorToolkit : public FAssetEditorToolkit
{
public:
	void InitEditor(const TArray<UObject*>& InObjects);
	
	void RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
	void UnregisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
 
	FName GetToolkitFName() const override { return "DialogueEditor"; }
	FText GetBaseToolkitName() const override { return INVTEXT("Dialogue Editor"); }
	FString GetWorldCentricTabPrefix() const override { return "Dialogue Distribution "; }
	FLinearColor GetWorldCentricTabColorScale() const override { return {}; }


	//Saves the graph data
	void UpdateDialogueAssetFromGraph();
	//Loads the graph data
	void UpdateGraphFromDialogueAsset();
	
	virtual void SaveAsset_Execute() override;
protected:

	UDialogue* Dialogue = nullptr;
	TObjectPtr<UDialogueGraph> DialogueGraph = nullptr;
	TSharedPtr<SDialogueGraphEditor> GraphEditor;
	TSharedPtr<IDetailsView> DetailsView;
	

};
