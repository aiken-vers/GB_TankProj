// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SSRadioButtons.h"
#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "RadioButtons.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRadioChoiceChangedEvent, FRadioChoice, NewRadioChoice);

UCLASS()
class TICKTANKS_API URadioButtons : public UWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings for test")
	uint8 DefaultCheckBoxIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings for test")
	uint8 ListSize = 3;
	
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;	
	void AddValueToSize(int Value);	

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

	void HandleOnRadioChoiceChanged(FRadioChoice NewRadioChoice) const;

	uint8 GetDefaultCheckBox() const;
	uint8 GetDefaultSize() const;

public:
	UPROPERTY(BlueprintAssignable)
	FOnRadioChoiceChangedEvent OnRadioChoiceChanged;
	
protected:
	TSharedPtr<SSRadioButtons> MyRadioButtons;
	
};
