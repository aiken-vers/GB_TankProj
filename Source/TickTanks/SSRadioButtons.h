// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <list>

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

UENUM(BlueprintType)
enum class ERadioChoice : uint8
{
	Radio0,
	Radio1,
	Radio2,
};

DECLARE_DELEGATE_OneParam(FOnRadioChoiceChanged, ERadioChoice)

class TICKTANKS_API SSRadioButtons : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSRadioButtons)
	{}

	SLATE_EVENT(FOnRadioChoiceChanged, OnRadioChoiceChanged)
	
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	//void ChangeDefaultCheckBox(uint8 NewIndex);
	
protected:
	ECheckBoxState IsRadioButtonChecked(ERadioChoice RadioButtonID);
	void HandleRadioButtonStateChanged(ECheckBoxState NewRadioState, ERadioChoice RadioButtonID);
	TSharedRef<SWidget> CreateRadioButton(const FString& RadioText, ERadioChoice RadioButtonChoice);	

protected:
	ERadioChoice CurrentChoice;
	FOnRadioChoiceChanged OnRadioChoiceChanged;
	//uint8 DefaultCheckBox = 0;
	//uint8 DefaultSize = 3;
};
