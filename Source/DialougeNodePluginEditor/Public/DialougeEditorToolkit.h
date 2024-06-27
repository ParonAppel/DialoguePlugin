// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialougeGraph.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "DialougeNodePlugin/Public/Dialouge.h"

/**
 * 
 */
class DIALOUGENODEPLUGINEDITOR_API FDialougeEditorToolkit : public FAssetEditorToolkit
{
public:
	void InitEditor(const TArray<UObject*>& InObjects);
	
	void RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
	void UnregisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
 
	FName GetToolkitFName() const override { return "DialougeEditor"; }
	FText GetBaseToolkitName() const override { return INVTEXT("Dialouge Editor"); }
	FString GetWorldCentricTabPrefix() const override { return "Dialouge Distribution "; }
	FLinearColor GetWorldCentricTabColorScale() const override { return {}; }

	//Saves the graph data
	void UpdateDialougeAssetFromGraph();
	//Loads the graph data
	void UpdateGraphFromDialougeAsset();
	
	virtual void SaveAsset_Execute() override;
protected:

	UDialouge* Dialouge = nullptr;
	UEdGraph* DialougeGraph = nullptr;
};
