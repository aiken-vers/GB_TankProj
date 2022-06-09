// Fill out your copyright notice in the Description page of Project Settings.


#include "RadioButtons.h"

void URadioButtons::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MyRadioButtons.Reset();
}

TSharedRef<SWidget> URadioButtons::RebuildWidget()
{
	MyRadioButtons = SNew(SSRadioButtons)
	.OnRadioChoiceChanged(BIND_UOBJECT_DELEGATE(FOnRadioChoiceChanged, HandleOnRadioChoiceChanged));

	return MyRadioButtons.ToSharedRef();	
}

void URadioButtons::HandleOnRadioChoiceChanged(ERadioChoice NewRadioChoice)
{
	if(!OnRadioChoiceChanged.IsBound())
		return;
	
	OnRadioChoiceChanged.Broadcast(NewRadioChoice);
}


