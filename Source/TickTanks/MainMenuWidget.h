// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RadioButtons.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "MainMenuWidget.generated.h"


UCLASS()
class TICKTANKS_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	
protected:	
	UPROPERTY(meta = (BindWidgetOptional))
	UButton* StartButton;
	UPROPERTY(meta = (BindWidgetOptional))
	UButton* SwitchToRadio;
	UPROPERTY(meta = (BindWidgetOptional))
	UButton* FirstLevel;
	UPROPERTY(meta = (BindWidgetOptional))
	UButton* SecondLevel;
	UPROPERTY(meta = (BindWidgetOptional))
	UButton* ThirdLevel;
	UPROPERTY(meta = (BindWidgetOptional))
	UButton* QuitGame;
	UPROPERTY(meta = (BindWidgetOptional))
	USlider* MusicSlider;
	UPROPERTY(meta = (BindWidgetOptional))
	UWidgetSwitcher* MenuWidgetSwitcher;
	UPROPERTY(meta = (BindWidgetOptional))
	URadioButtons* RadioButtonsWidget;
	UPROPERTY(meta = (BindWidgetOptional))
	UButton* AddRadio;
	UPROPERTY(meta = (BindWidgetOptional))
	UButton* RemoveRadio;

	UPROPERTY(BlueprintReadOnly,Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* Lvl1Hover;
	UPROPERTY(BlueprintReadOnly,Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* Lvl2Hover;
	UPROPERTY(BlueprintReadOnly,Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* Lvl3Hover;
	
protected:
	UFUNCTION()
	void OpenFirst();
	UFUNCTION()
	void HoverFirst();
	UFUNCTION()
	void UnHoverFirst();
	
	UFUNCTION()
	void OpenSecond();
	UFUNCTION()
	void HoverSecond();
	UFUNCTION()
	void UnHoverSecond();
	
	UFUNCTION()
	void OpenThird();
	UFUNCTION()
	void HoverThird();
	UFUNCTION()
	void UnHoverThird();

	UFUNCTION()
	void ChangeMusicVolume(float Value);
	UFUNCTION()
	void ChangeSoundVolume(float Value, FName SoundClassPath);
	
	UFUNCTION()
	void Quit();

	UFUNCTION()
	void SwitchToRadioButtons();

	UFUNCTION()
	void AddRadioButtons();
	UFUNCTION()
	void RemoveRadioButtons();
	
private:
	FTimerHandle LoadTimer;
	void OpenLevel();
	void SwitchMenu(int32 WidgetIndex);
	FName LevelName ="DefaultMap";
	FName MusicPath = "/Game/CSC/Demo/Sounds/Music.Music";
};




