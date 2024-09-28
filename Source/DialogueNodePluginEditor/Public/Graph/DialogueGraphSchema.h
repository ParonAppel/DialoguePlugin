// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "DialogueGraphSchema.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGUENODEPLUGINEDITOR_API UDialogueGraphSchema : public UEdGraphSchema
{
	GENERATED_BODY()

public:
	
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;

	virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;

protected:

	void CreateStartNode(UEdGraph& Graph) const;
};


// ----------------- Actions ------------------
USTRUCT()
struct FNewSimpleDialogueNodeAction : public FEdGraphSchemaAction
{
	GENERATED_BODY()

public:

	FNewSimpleDialogueNodeAction() {}
	FNewSimpleDialogueNodeAction(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping)
		: FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping) {}

	virtual UEdGraphNode* PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode) override;
};

USTRUCT()
struct FNewPlayerOptionsNodeAction : public FEdGraphSchemaAction
{
	GENERATED_BODY()

public:

	FNewPlayerOptionsNodeAction() {}
	FNewPlayerOptionsNodeAction(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping)
		: FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping) {}

	virtual UEdGraphNode* PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode) override;
};

USTRUCT()
struct FNewTagBranchNodeAction : public FEdGraphSchemaAction
{
	GENERATED_BODY()

public:

	FNewTagBranchNodeAction() {}
	FNewTagBranchNodeAction(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping)
		: FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping) {}

	virtual UEdGraphNode* PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode) override;
};

USTRUCT()
struct FNewEventNodeAction : public FEdGraphSchemaAction
{
	GENERATED_BODY()

public:

	FNewEventNodeAction() {}
	FNewEventNodeAction(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping)
		: FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping) {}

	virtual UEdGraphNode* PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode) override;
};
