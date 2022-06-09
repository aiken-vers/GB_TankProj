// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include <vector>
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "SSRadioButtons.generated.h"


USTRUCT(BlueprintType)
struct FRadioChoice
{
	GENERATED_BODY()
/*
	FORCEINLINE FRadioChoice()
	{
		Name = TEXT("Radio");
		Index = 0;
	}
	explicit FORCEINLINE FRadioChoice(FName FValue, uint8 IValue)
	{
		Name = FValue;
		Index = IValue;
	}
*/
	
	UPROPERTY(BlueprintReadWrite)
	FString Name;
	UPROPERTY(BlueprintReadWrite)
	uint8 Index;

	bool operator == (const FRadioChoice& Other) const
	{
		return Index == Other.Index;
	}
	
};

DECLARE_DELEGATE_OneParam(FOnRadioChoiceChanged, FRadioChoice)

class TICKTANKS_API SSRadioButtons : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSRadioButtons)
	{}

	SLATE_EVENT(FOnRadioChoiceChanged, OnRadioChoiceChanged)
	SLATE_ATTRIBUTE(uint8, DefaultCheckBox)
	SLATE_ATTRIBUTE(uint8, DefaultSize)
	
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	//
	// Скопировано со слайдера т.к. атрибуты обновлялись при компиляции, но не в реалтайме	
	void SetSize(uint8 Value);
	void SetChoice(uint8 Value);
	
protected:
	ECheckBoxState IsRadioButtonChecked(FRadioChoice RadioButtonID);
	void HandleRadioButtonStateChanged(ECheckBoxState NewRadioState, FRadioChoice RadioButtonID);
	TSharedRef<SWidget> CreateRadioButton(FRadioChoice RadioButtonChoice);
	void CheckListSize();
	void AddRadioButton(uint8 Index);
	void LoadAllButtons();

public:	
	std::vector<FRadioChoice> RadioButtonsList;
	TSharedRef<SVerticalBox> VerticalBoxRef = SNew(SVerticalBox);	
protected:	
	FRadioChoice CurrentChoice;
	FOnRadioChoiceChanged OnRadioChoiceChanged;	
	TAttribute<uint8> DefaultCheckBox;
	TAttribute<uint8> DefaultSize;
};
