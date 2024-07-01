// Fill out your copyright notice in the Description page of Project Settings.


#include "DialougeEditorToolkit.h"

#include "Graph/DialougeGraphEditor.h"
#include "Nodes/DialougeGraphNode.h"
#include "Graph/DialougeGraphSchema.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Widgets/Docking/SDockTab.h"
#include "Modules/ModuleManager.h"


void FDialougeEditorToolkit::InitEditor(const TArray<UObject*>& InObjects)
{
	Dialouge = Cast<UDialouge>(InObjects[0]);

	DialougeGraph = Cast<UDialougeGraph>(FBlueprintEditorUtils::CreateNewGraph(Dialouge,"DialougeGraph", UDialougeGraph::StaticClass(), UDialougeGraphSchema::StaticClass()));

	DialougeGraph->DialougeAsset = Dialouge;
	
	const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout("DialougeEditorLayout")
	->AddArea
	(
		FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
		->Split
		(
			FTabManager::NewSplitter()
			->SetSizeCoefficient(0.6f)
			->SetOrientation(Orient_Horizontal)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.8f)
				->AddTab("DialougeGraphTab", ETabState::OpenedTab)
			)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.2f)
				->AddTab("DialougeDetailsTab", ETabState::OpenedTab)
			)
		)
	);

	FAssetEditorToolkit::InitAssetEditor(EToolkitMode::Standalone, {}, "DialougeEditor", Layout, true, true, InObjects);
}

void FDialougeEditorToolkit::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(INVTEXT("Dialouge Editor"));
 
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DetailsView->SetObjects(TArray<UObject*>{ Dialouge });
	InTabManager->RegisterTabSpawner("DialougeDetailsTab", FOnSpawnTab::CreateLambda([this](const FSpawnTabArgs&)
	{
		return SNew(SDockTab)
		[
			DetailsView.ToSharedRef()
		];
	}))
	.SetDisplayName(INVTEXT("Details"))
	.SetGroup(WorkspaceMenuCategory.ToSharedRef());

	InTabManager->RegisterTabSpawner("DialougeGraphTab", FOnSpawnTab::CreateLambda([this](const FSpawnTabArgs&)
	{
		return SNew(SDockTab)
			[
				SAssignNew(GraphEditor, SDialougeGraphEditor, DialougeGraph)
					.DetailsView(DetailsView)
			];
	}))
	.SetDisplayName(INVTEXT("Graph"))
	.SetGroup(WorkspaceMenuCategory.ToSharedRef());

	UpdateGraphFromDialougeAsset();
	
}

void FDialougeEditorToolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
	InTabManager->UnregisterTabSpawner("DialougeGraphTab");
	InTabManager->UnregisterTabSpawner("DialougeDetailsTab");
}

void FDialougeEditorToolkit::UpdateDialougeAssetFromGraph()
{
	if(!Dialouge || !DialougeGraph)
		return;

	UDialougeRuntimeGraph* runtimeGraph = NewObject<UDialougeRuntimeGraph>(Dialouge);
	Dialouge->Graph = runtimeGraph;

	TArray<std::pair<FGuid, FGuid>> connections;
	TMap<FGuid, UDialougeRuntimePin*> idToPinMap;

	//Save Nodes
	for(UEdGraphNode* uiNode : DialougeGraph->Nodes)
	{
		
		UDialougeRuntimeNode* runtimeNode = NewObject<UDialougeRuntimeNode>(runtimeGraph);
		runtimeNode->Position = FVector2D(uiNode->NodePosX, uiNode->NodePosY);
		runtimeNode->NodeClass = uiNode->GetClass();
		
		if(UDialougeGraphNode* dialougeUINode = Cast<UDialougeGraphNode>(uiNode))
			runtimeNode->NodeSaveData = dialougeUINode->Data;

		for (UEdGraphPin* pin : uiNode->Pins)
		{
			UDialougeRuntimePin* runtimePin = NewObject<UDialougeRuntimePin>(runtimeNode);
			runtimePin->PinName = pin->PinName;
			runtimePin->PinID = pin->PinId;

			if(pin->HasAnyConnections() && pin->Direction == EGPD_Output)
			{
				std::pair<FGuid, FGuid> connection = std::make_pair(pin->PinId, pin->LinkedTo[0]->PinId);
				connections.Add(connection);
			}

			idToPinMap.Add(pin->PinId, runtimePin);

			if(pin->Direction == EGPD_Input)
			{
				runtimeNode->InputPin = runtimePin;
			}
			else
			{
				runtimeNode->OutputPins.Add(runtimePin);
			}
		}

		runtimeGraph->Nodes.Add(runtimeNode);
	}

	//Save Connections
	for(std::pair<FGuid, FGuid> connection : connections)
	{
		UDialougeRuntimePin* pin1 = idToPinMap[connection.first];
		UDialougeRuntimePin* pin2 = idToPinMap[connection.second];

		pin1->Connection = pin2;
	}
}

void FDialougeEditorToolkit::UpdateGraphFromDialougeAsset()
{
	if(!Dialouge || !DialougeGraph)
		return;

	if(!Dialouge->Graph)
		return;
	
	TArray<std::pair<FGuid, FGuid>> connections;
	TMap<FGuid, UEdGraphPin*> idToPinMap;

	//Remake nodes
	for(UDialougeRuntimeNode* runtimeNode : Dialouge->Graph->Nodes )
	{
		UDialougeGraphNode* newUINode = NewObject<UDialougeGraphNode>(DialougeGraph.Get(), runtimeNode->NodeClass);
		newUINode->CreateNewGuid();
		newUINode->NodePosX = runtimeNode->Position.X;
		newUINode->NodePosY = runtimeNode->Position.Y;
		
		newUINode->SetSaveData(runtimeNode->NodeSaveData);

		if(runtimeNode->InputPin != nullptr)
		{
			UDialougeRuntimePin* runtimePin = runtimeNode->InputPin;
			UEdGraphPin* uiPin = newUINode->CreateCustomPin(EGPD_Input, runtimePin->PinName);
			uiPin->PinId = runtimePin->PinID;

			if(runtimePin->Connection)
			{
				connections.Add(std::make_pair(runtimePin->PinID, runtimePin->Connection->PinID));
			}

			idToPinMap.Add(runtimePin->PinID, uiPin);
		}

		for(UDialougeRuntimePin* runtimePin : runtimeNode->OutputPins)
		{
			UEdGraphPin* uiPin = newUINode->CreateCustomPin(EGPD_Output, runtimePin->PinName);
			uiPin->PinId = runtimePin->PinID;

			if(runtimePin->Connection)
			{
				connections.Add(std::make_pair(runtimePin->PinID, runtimePin->Connection->PinID));
			}
			idToPinMap.Add(runtimePin->PinID, uiPin);
		}

		DialougeGraph->AddNode(newUINode, true, true);
	}

	//Remake Connections
	for(std::pair<FGuid, FGuid> connection : connections)
	{
		UEdGraphPin* pin1 = idToPinMap[connection.first];
		UEdGraphPin* pin2 = idToPinMap[connection.second];

		pin1->LinkedTo.Add(pin2);
		pin2->LinkedTo.Add(pin1);
	}
}

void FDialougeEditorToolkit::SaveAsset_Execute()
{
	UpdateDialougeAssetFromGraph();
	FAssetEditorToolkit::SaveAsset_Execute();
}



