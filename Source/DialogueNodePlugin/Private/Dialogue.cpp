// Fill out your copyright notice in the Description page of Project Settings.


#include "Dialogue.h"


bool UDialogue::BindToDelegateByName(FName DelegateName, FTest const& event)
{
	for( int i = 0; i < Delegates.Num(); i++ )
	{
		if(DelegateName != Delegates[i].DelegateName)
			continue;

		Delegates[i].Delegate = event;
		return Delegates[i].Delegate.IsBound();
	}
	return false;
}

bool UDialogue::BindToDialogueEvent(FName DelegateName, FTest const& event)
{
	for( int i = 0; i < Delegates.Num(); i++ )
	{
		if(DelegateName != Delegates[i].DelegateName)
			continue;

		Delegates[i].Delegate = event;
		return Delegates[i].Delegate.IsBound();
	}
	return false;
}

bool UDialogue::BindToDelegateByIndex(int Index, FTest const& event)
{
	if(!Delegates.IsValidIndex(Index))
		return false;

	Delegates[Index].Delegate = event;
	return true;
}


bool UDialogue::CallOnDelegateByName(const FName& DelegateName)
{
	for(FDialogueDelegate dialogueDelegate : Delegates)
	{
		if(DelegateName != dialogueDelegate.DelegateName)
			continue;
		
		return dialogueDelegate.Delegate.ExecuteIfBound();
	}

	return false;
}

TArray<FName> UDialogue::GetOptions() 
{
	TArray<FName> Names;
	for( int i = 0; i < Delegates.Num(); i++ )
	{
		Names.Add(Delegates[i].DelegateName);
	}
	return Names;
}
