// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialougeActions.h"
#include "EdGraphUtilities.h"
#include "SGraphPin.h"
#include "Modules/ModuleManager.h"

static EAssetTypeCategories::Type DialougeCategory;
static TSharedPtr<FSlateStyleSet> StyleSet;

class SDialougeGraphPin : public SGraphPin
{
public:

	SLATE_BEGIN_ARGS(SDialougeGraphPin){}
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

struct FDialougePinFactory : FGraphPanelPinFactory
{
public:
	virtual ~FDialougePinFactory() {}
	virtual TSharedPtr<SGraphPin> CreatePin(UEdGraphPin* Pin) const override
	{
		if(FName(TEXT("DialougePin")) == Pin->PinType.PinSubCategory)
		{
			return SNew(SDialougeGraphPin, Pin);
		}

		return nullptr;
	}
};


class FDialougeNodePluginEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	
private:
	
	TSharedPtr<FDialougeActions> DialougeActions = nullptr;
	TSharedPtr<FDialougePinFactory> PinFactory = nullptr;
	
};

