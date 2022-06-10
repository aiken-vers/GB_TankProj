// Fill out your copyright notice in the Description page of Project Settings.


#include "SSRadioButtons.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSRadioButtons::Construct(const FArguments& InArgs)
{
	DefaultSize = 3;
	DefaultCheckBox = 0;	
	
	OnRadioChoiceChanged = InArgs._OnRadioChoiceChanged;
	DefaultCheckBox = InArgs._DefaultCheckBox;
	DefaultSize = InArgs._DefaultSize;
	
	CheckListSize();
	CurrentChoice = RadioButtonsList[DefaultCheckBox.Get()];
	
	ChildSlot
	[		
		SAssignNew(VerticalBoxRef, SVerticalBox)
	];
	LoadAllButtons();
}

void SSRadioButtons::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
	
	if(RadioButtonsList.size()==DefaultSize.Get())
		return;

	CheckListSize();
	LoadAllButtons();	
}

ECheckBoxState SSRadioButtons::IsRadioButtonChecked(FRadioChoice RadioButtonID)
{	
	return(CurrentChoice==RadioButtonID)
	? ECheckBoxState::Checked
	: ECheckBoxState::Unchecked;	
}

void SSRadioButtons::HandleRadioButtonStateChanged(ECheckBoxState NewRadioState, FRadioChoice RadioButtonID)
{
	if(NewRadioState == ECheckBoxState::Checked)
	{
		CurrentChoice = RadioButtonID;
		OnRadioChoiceChanged.ExecuteIfBound(CurrentChoice);
	}	
}

TSharedRef<SWidget> SSRadioButtons::CreateRadioButton(FRadioChoice RadioButtonChoice)
{
	return SNew(SCheckBox)
	.IsChecked(MakeAttributeRaw(this, &SSRadioButtons::IsRadioButtonChecked, RadioButtonChoice))
	.OnCheckStateChanged(this, &SSRadioButtons::HandleRadioButtonStateChanged, RadioButtonChoice)
	[
		SNew(STextBlock)
		.Text(FText::FromString(RadioButtonChoice.Name))
	];
}

void SSRadioButtons::CheckListSize()
{
	while(RadioButtonsList.size()<DefaultSize.Get())
	{
		uint8 Index = RadioButtonsList.size();
		FString Tempname = "Radio"+FString::FromInt(Index);
		FRadioChoice NewChoice {Tempname, Index};
		RadioButtonsList.push_back(NewChoice);
	}
	while(RadioButtonsList.size()>DefaultSize.Get())
	{
		RadioButtonsList.pop_back();
	}
}

void SSRadioButtons::AddRadioButton(uint8 Index)
{
	VerticalBoxRef->AddSlot()
	[
		CreateRadioButton(RadioButtonsList[Index])
	];
}

void SSRadioButtons::LoadAllButtons()
{
	VerticalBoxRef->ClearChildren();
	for(uint8 i = 0; i < RadioButtonsList.size(); i++)
	{
		AddRadioButton(i);
	}
	if(CurrentChoice.Index>=RadioButtonsList.size())
		CurrentChoice = RadioButtonsList[RadioButtonsList.size()-1];
}

void SSRadioButtons::SetSize(uint8 Value)
{
	DefaultSize = Value;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
