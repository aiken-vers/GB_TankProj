// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SSRadioButtons.h"
#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "RadioButtons.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRadioChoiceChangedEvent, ERadioChoice, NewRadioChoice);

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
	//virtual void OnEndEditByDesigner() override;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

	void HandleOnRadioChoiceChanged(ERadioChoice NewRadioChoice);

public:
	UPROPERTY(BlueprintAssignable)
	FOnRadioChoiceChangedEvent OnRadioChoiceChanged;
	
protected:
	TSharedPtr<SSRadioButtons> MyRadioButtons;
	
};
