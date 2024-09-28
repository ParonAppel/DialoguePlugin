// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueGraphRuntime.h"
#include "Templates/Function.h"
#include "Dialogue.generated.h"

/**
 * 
 */
USTRUCT(Blueprintable)
struct FCastMember  : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name = INVTEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor Color = FColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> Portrait = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDialougeEventSignature);
DECLARE_DYNAMIC_DELEGATE(FTest);

USTRUCT(Blueprintable)
struct FDialogueDelegate
{
	GENERATED_BODY()
public:

	
	UPROPERTY(BlueprintReadOnly)
	FTest Delegate;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName DelegateName = NAME_None;
	
};

UCLASS()
class DIALOGUENODEPLUGIN_API UDialogue : public UObject
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable)
	bool BindToDelegateByName(FName DelegateName , FTest const& event);

	UFUNCTION(BlueprintCallable)
	bool BindToDialogueEvent(UPARAM(meta=(GetOptions="GetOptions"))FName DelegateName , FTest const& event);

	UFUNCTION(BlueprintCallable)
	bool BindToDelegateByIndex(int Index , FTest const& event);

	UFUNCTION()
	bool CallOnDelegateByName(const FName& DelegateName);
	
	UPROPERTY()
	UDialogueRuntimeGraph* Graph = nullptr;

protected:

	UFUNCTION()
	TArray<FName> GetOptions();
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FDialogueDelegate> Delegates;
};
