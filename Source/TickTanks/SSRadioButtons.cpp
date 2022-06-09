// Fill out your copyright notice in the Description page of Project Settings.


#include "SSRadioButtons.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSRadioButtons::Construct(const FArguments& InArgs)
{
	OnRadioChoiceChanged = InArgs._OnRadioChoiceChanged;
	
	CurrentChoice = ERadioChoice::Radio0;
	
	ChildSlot
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		[
			//CreateRadioButton("Option0", ERadioChoice::Radio0)
			CreateRadioButton("Option0", ERadioChoice::Radio0)
		]
		+SVerticalBox::Slot()
		[
			CreateRadioButton("Option1", ERadioChoice::Radio1)
		]
		+SVerticalBox::Slot()
		[
			CreateRadioButton("Option2", ERadioChoice::Radio2)
		]
	];
	
}

ECheckBoxState SSRadioButtons::IsRadioButtonChecked(ERadioChoice RadioButtonID)
{
	return(CurrentChoice==RadioButtonID)
	? ECheckBoxState::Checked
	: ECheckBoxState::Unchecked;
}

void SSRadioButtons::HandleRadioButtonStateChanged(ECheckBoxState NewRadioState, ERadioChoice RadioButtonID)
{
	if(NewRadioState == ECheckBoxState::Checked)
	{
		CurrentChoice = RadioButtonID;
		OnRadioChoiceChanged.ExecuteIfBound(CurrentChoice);
	}	
}

TSharedRef<SWidget> SSRadioButtons::CreateRadioButton(const FString& RadioText, ERadioChoice RadioButtonChoice)
{
	return SNew(SCheckBox)
	.IsChecked(MakeAttributeRaw(this, &SSRadioButtons::IsRadioButtonChecked, RadioButtonChoice))
	.OnCheckStateChanged(this, &SSRadioButtons::HandleRadioButtonStateChanged, RadioButtonChoice)
	[
		SNew(STextBlock)
		.Text(FText::FromString(RadioText))
	];
}
/*
void SSRadioButtons::ChangeDefaultCheckBox(uint8 NewIndex)
{
	
	if(DefaultSize<NewIndex+1)
		return;
	DefaultCheckBox = NewIndex;
	
}
*/

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
