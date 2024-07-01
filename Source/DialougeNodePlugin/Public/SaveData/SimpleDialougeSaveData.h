// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dialouge.h"
#include "DialougeNodeSaveData.h"
#include "SimpleDialougeSaveData.generated.h"

/**
 * 
 */
UCLASS()
class DIALOUGENODEPLUGIN_API USimpleDialougeSaveData : public UDialougeNodeSaveData
{
	GENERATED_BODY()

public:
	USimpleDialougeSaveData();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(RowType="CastMember"), Category="Cast")
	TObjectPtr<UDataTable> CastDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(GetOptions="GetRowOptions", EditConditionHides, EditCondition="CastDataTable != nullptr"), Category="Cast");
	FName CastMemberRowName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="CastDataTable == nullptr"), Category="Cast");
	FCastMember CastMember;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(MultiLine=true), Category="Dialouge")
	FText DialougeText;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
private:

	UFUNCTION()
	TArray<FString> GetRowOptions();
	
};
