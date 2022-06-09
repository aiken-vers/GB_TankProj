// Fill out your copyright notice in the Description page of Project Settings.


#include "RadioButtons.h"

#include "IContentBrowserSingleton.h"

void URadioButtons::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MyRadioButtons.Reset();
}

void URadioButtons::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if(ListSize<1)
		ListSize=1;
		
	if(DefaultCheckBoxIndex>=ListSize)
		DefaultCheckBoxIndex = ListSize-1;
	
}

void URadioButtons::AddValueToSize(int Value)
{
	ListSize+=Value;
	
}

TSharedRef<SWidget> URadioButtons::RebuildWidget()
{
	MyRadioButtons = SNew(SSRadioButtons)
	.OnRadioChoiceChanged(BIND_UOBJECT_DELEGATE(FOnRadioChoiceChanged, HandleOnRadioChoiceChanged))
	.DefaultCheckBox(GetDefaultCheckBox())
	.DefaultSize(GetDefaultSize());

	return MyRadioButtons.ToSharedRef();	
}

void URadioButtons::HandleOnRadioChoiceChanged(FRadioChoice NewRadioChoice) const
{
	if(!OnRadioChoiceChanged.IsBound())
		return;
	
	OnRadioChoiceChanged.Broadcast(NewRadioChoice);
}

uint8 URadioButtons::GetDefaultCheckBox() const
{
	return DefaultCheckBoxIndex;
}

uint8 URadioButtons::GetDefaultSize() const
{
	return ListSize;
}


