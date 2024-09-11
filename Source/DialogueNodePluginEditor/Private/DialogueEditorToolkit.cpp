// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueEditorToolkit.h"

#include "Graph/DialogueGraphEditor.h"
#include "Nodes/DialogueGraphNode.h"
#include "Graph/DialogueGraphSchema.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Widgets/Docking/SDockTab.h"
#include "Modules/ModuleManager.h"
#include "Nodes/StartDialogueGraphNode.h"


void FDialogueEditorToolkit::InitEditor(const TArray<UObject*>& InObjects)
{
	Dialogue = Cast<UDialogue>(InObjects[0]);

	DialogueGraph = Cast<UDialogueGraph>(FBlueprintEditorUtils::CreateNewGraph(Dialogue,"DialogueGraph", UDialogueGraph::StaticClass(), UDialogueGraphSchema::StaticClass()));
	DialogueGraph->bAllowDeletion = false;
	DialogueGraph->DialogueAsset = Dialogue;
	
	const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout("DialogueEditorLayout")
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
				->AddTab("DialogueGraphTab", ETabState::OpenedTab)
			)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.2f)
				->AddTab("DialogueDetailsTab", ETabState::OpenedTab)
			)
		)
	);

	FAssetEditorToolkit::InitAssetEditor(EToolkitMode::Standalone, {}, "DialogueEditor", Layout, true, true, InObjects);
}

void FDialogueEditorToolkit::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(INVTEXT("Dialogue Editor"));
 
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DetailsView->SetObjects(TArray<UObject*>{ Dialogue });
	InTabManager->RegisterTabSpawner("DialogueDetailsTab", FOnSpawnTab::CreateLambda([this](const FSpawnTabArgs&)
	{
		return SNew(SDockTab)
		[
			DetailsView.ToSharedRef()
		];
	}))
	.SetDisplayName(INVTEXT("Details"))
	.SetGroup(WorkspaceMenuCategory.ToSharedRef());

	InTabManager->RegisterTabSpawner("DialogueGraphTab", FOnSpawnTab::CreateLambda([this](const FSpawnTabArgs&)
	{
		return SNew(SDockTab)
			[
				SAssignNew(GraphEditor, SDialogueGraphEditor, DialogueGraph)
					.DetailsView(DetailsView)
			];
	}))
	.SetDisplayName(INVTEXT("Graph"))
	.SetGroup(WorkspaceMenuCategory.ToSharedRef());

	if(!Dialogue->Graph || !Dialogue->Graph->StartNode)
	{
		DialogueGraph->GetSchema()->CreateDefaultNodesForGraph(*DialogueGraph);
	}
	UpdateGraphFromDialogueAsset();
	
}

void FDialogueEditorToolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
	InTabManager->UnregisterTabSpawner("DialogueGraphTab");
	InTabManager->UnregisterTabSpawner("DialogueDetailsTab");
}

void FDialogueEditorToolkit::UpdateDialogueAssetFromGraph()
{
	if(!Dialogue || !DialogueGraph)
		return;

	UDialogueRuntimeGraph* runtimeGraph = NewObject<UDialogueRuntimeGraph>(Dialogue);
	Dialogue->Graph = runtimeGraph;

	TArray<std::pair<FGuid, FGuid>> connections;
	TMap<FGuid, UDialogueRuntimePin*> idToPinMap;

	//Save Nodes
	for(UEdGraphNode* uiNode : DialogueGraph->Nodes)
	{
		
		UDialogueRuntimeNode* runtimeNode = NewObject<UDialogueRuntimeNode>(runtimeGraph);
		runtimeNode->Position = FVector2D(uiNode->NodePosX, uiNode->NodePosY);
		runtimeNode->NodeClass = uiNode->GetClass();
		
		if(UDialogueGraphNode* DialogueUINode = Cast<UDialogueGraphNode>(uiNode))
			runtimeNode->NodeSaveData = DialogueUINode->Data;

		if(uiNode->GetClass() == UStartDialogueGraphNode::StaticClass())
			Dialogue->Graph->StartNode = runtimeNode;

		for (UEdGraphPin* pin : uiNode->Pins)
		{
			UDialogueRuntimePin* runtimePin = NewObject<UDialogueRuntimePin>(runtimeNode);
			runtimePin->PinName = pin->PinName;
			runtimePin->PinID = pin->PinId;
			runtimePin->Owner = runtimeNode;
			
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
		UDialogueRuntimePin* pin1 = idToPinMap[connection.first];
		UDialogueRuntimePin* pin2 = idToPinMap[connection.second];

		pin1->Connection = pin2;
	}
}

void FDialogueEditorToolkit::UpdateGraphFromDialogueAsset()
{
	if(!Dialogue || !DialogueGraph)
		return;

	if(!Dialogue->Graph)
		return;
	
	TArray<std::pair<FGuid, FGuid>> connections;
	TMap<FGuid, UEdGraphPin*> idToPinMap;

	//Remake nodes
	for(UDialogueRuntimeNode* runtimeNode : Dialogue->Graph->Nodes )
	{
		UDialogueGraphNode* newUINode = NewObject<UDialogueGraphNode>(DialogueGraph.Get(), runtimeNode->NodeClass);
		newUINode->CreateNewGuid();
		newUINode->NodePosX = runtimeNode->Position.X;
		newUINode->NodePosY = runtimeNode->Position.Y;
		
		newUINode->SetSaveData(runtimeNode->NodeSaveData);

		if(runtimeNode->InputPin != nullptr)
		{
			UDialogueRuntimePin* runtimePin = runtimeNode->InputPin;
			UEdGraphPin* uiPin = newUINode->CreateCustomPin(EGPD_Input, runtimePin->PinName);
			uiPin->PinId = runtimePin->PinID;

			if(runtimePin->Connection)
			{
				connections.Add(std::make_pair(runtimePin->PinID, runtimePin->Connection->PinID));
			}

			idToPinMap.Add(runtimePin->PinID, uiPin);
		}

		
		
		for(UDialogueRuntimePin* runtimePin : runtimeNode->OutputPins)
		{
			UEdGraphPin* uiPin = newUINode->CreateCustomPin(EGPD_Output, runtimePin->PinName);
			uiPin->PinId = runtimePin->PinID;

			if(runtimePin->Connection)
			{
				connections.Add(std::make_pair(runtimePin->PinID, runtimePin->Connection->PinID));
			}
			idToPinMap.Add(runtimePin->PinID, uiPin);

			
		}
		
		DialogueGraph->AddNode(newUINode, true, true);
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

void FDialogueEditorToolkit::SaveAsset_Execute()
{
	UpdateDialogueAssetFromGraph();
	FAssetEditorToolkit::SaveAsset_Execute();
}



