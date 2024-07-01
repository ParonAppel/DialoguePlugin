// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveData/SimpleDialougeSaveData.h"
#include "UObject/Object.h"


USimpleDialougeSaveData::USimpleDialougeSaveData()
{
}

void USimpleDialougeSaveData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if(PropertyChangedEvent.Property->GetName() == "CastMemberRowName" && CastDataTable)
	{
		FCastMember* foundMember = CastDataTable.Get()->FindRow<FCastMember>(CastMemberRowName,"");
		if(foundMember)
			CastMember = *foundMember;
	}
	
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

TArray<FString> USimpleDialougeSaveData::GetRowOptions()
{
	TArray<FString> options;

	if(CastDataTable)
	{
		for(FName rowName : CastDataTable->GetRowNames())
		{
			options.Add(rowName.ToString());
		}
	}

	return options;
}


