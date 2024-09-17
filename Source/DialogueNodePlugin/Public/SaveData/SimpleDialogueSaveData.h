// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dialogue.h"
#include "DialogueNodeSaveData.h"
#include "SimpleDialogueSaveData.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGUENODEPLUGIN_API USimpleDialogueSaveData : public UDialogueNodeSaveData
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(RowType="CastMember"), Category="Cast")
	TObjectPtr<UDataTable> CastDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(GetOptions="GetRowOptions", EditConditionHides, EditCondition="CastDataTable != nullptr"), Category="Cast");
	FName CastMemberRowName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="CastDataTable == nullptr"), Category="Cast");
	FCastMember CastMember;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(MultiLine=true), Category="Dialogue")
	FText DialogueText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(MultiLine=true), Category="Dialogue")
	TObjectPtr<USoundWave> DialogueSound;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
private:

	UFUNCTION()
	TArray<FString> GetRowOptions();
	
};
