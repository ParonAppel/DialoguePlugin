// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueActions.h"
#include "EdGraphUtilities.h"
#include "SGraphPin.h"
#include "Modules/ModuleManager.h"

static EAssetTypeCategories::Type DialogueCategory;
static TSharedPtr<FSlateStyleSet> StyleSet;

class SDialogueGraphPin : public SGraphPin
{
public:

	SLATE_BEGIN_ARGS(SDialogueGraphPin){}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj)
	{
		SGraphPin::Construct(SGraphPin::FArguments(), InGraphPinObj);
	}

protected:
	virtual FSlateColor GetPinColor() const override
	{
		return FSlateColor(FLinearColor(0.5,0.5,0.5));
	}
};

struct FDialoguePinFactory : FGraphPanelPinFactory
{
public:
	virtual ~FDialoguePinFactory() {}
	virtual TSharedPtr<SGraphPin> CreatePin(UEdGraphPin* Pin) const override
	{
		if(FName(TEXT("DialoguePin")) == Pin->PinType.PinSubCategory)
		{
			return SNew(SDialogueGraphPin, Pin);
		}

		return nullptr;
	}
};


class FDialogueNodePluginEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	
private:
	
	TSharedPtr<FDialogueActions> DialogueActions = nullptr;
	TSharedPtr<FDialoguePinFactory> PinFactory = nullptr;
	
};

