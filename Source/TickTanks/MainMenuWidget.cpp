// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"

#include "StyleSet.h"


void UMainMenuWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	if(RadioButtonsWidget)
	{
		RadioButtonsWidget->WidgetStyle = FStyleSet::Get().GetWidgetStyle<FFRadioButtonStyle>(FName("RadioButtonStyle"));
	}
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();	
	if(StartButton)
		StartButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenFirst);
	
	if(FirstLevel)
	{
		FirstLevel->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenFirst);
		FirstLevel->OnHovered.AddDynamic(this, &UMainMenuWidget::HoverFirst);
		FirstLevel->OnUnhovered.AddDynamic(this, &UMainMenuWidget::UnHoverFirst);
	}
			

	if(SecondLevel)
	{
		SecondLevel->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenSecond);
		SecondLevel->OnHovered.AddDynamic(this, &UMainMenuWidget::HoverSecond);
		SecondLevel->OnUnhovered.AddDynamic(this, &UMainMenuWidget::UnHoverSecond);
	}		

	if(ThirdLevel)
	{
		ThirdLevel->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenThird);
		ThirdLevel->OnHovered.AddDynamic(this, &UMainMenuWidget::HoverThird);
		ThirdLevel->OnUnhovered.AddDynamic(this, &UMainMenuWidget::UnHoverThird);
	}		

	if(MusicSlider)
		MusicSlider->OnValueChanged.AddDynamic(this, &UMainMenuWidget::ChangeMusicVolume);

	if(SwitchToRadio)
		SwitchToRadio->OnClicked.AddDynamic(this, &UMainMenuWidget::SwitchToRadioButtons);

	if(AddRadio)
		AddRadio->OnClicked.AddDynamic(this, &UMainMenuWidget::AddRadioButtons);

	if(RemoveRadio)
		RemoveRadio->OnClicked.AddDynamic(this, &UMainMenuWidget::RemoveRadioButtons);

	if(QuitGame)
		QuitGame->OnClicked.AddDynamic(this, &UMainMenuWidget::Quit);	
}

void UMainMenuWidget::OpenLevel()
{	
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
	GetWorld()->GetTimerManager().ClearTimer(LoadTimer);
}

void UMainMenuWidget::OpenFirst()
{
	LevelName = "AssaultMap2";
	GetWorld()->GetTimerManager().SetTimer(LoadTimer, this, &UMainMenuWidget::OpenLevel, 1.0f, false);	
}

void UMainMenuWidget::HoverFirst()
{
	PlayAnimationForward(Lvl1Hover);
}
void UMainMenuWidget::UnHoverFirst()
{
	PlayAnimationReverse(Lvl1Hover);
}

void UMainMenuWidget::OpenSecond()
{
	LevelName = "AssaultMap1";
	GetWorld()->GetTimerManager().SetTimer(LoadTimer, this, &UMainMenuWidget::OpenLevel, 1.0f, false);
}
void UMainMenuWidget::HoverSecond()
{
	PlayAnimationForward(Lvl2Hover);
}
void UMainMenuWidget::UnHoverSecond()
{
	PlayAnimationReverse(Lvl2Hover);
}

void UMainMenuWidget::OpenThird()
{
	LevelName = "DefaultMap";
	GetWorld()->GetTimerManager().SetTimer(LoadTimer, this, &UMainMenuWidget::OpenLevel, 1.0f, false);	
}
void UMainMenuWidget::HoverThird()
{
	PlayAnimationForward(Lvl3Hover);
}
void UMainMenuWidget::UnHoverThird()
{
	PlayAnimationReverse(Lvl3Hover);
}

void UMainMenuWidget::ChangeMusicVolume(float Value)
{
	ChangeSoundVolume(Value, MusicPath);	
}

void UMainMenuWidget::ChangeSoundVolume(float Value, FName SoundClassPath)
{
	if (!SoundClassPath.IsValid())
		return;	
	
	USoundClass* SoundClass = LoadObject<USoundClass>(nullptr, *SoundClassPath.ToString());
	
	if(SoundClass!=nullptr)
		SoundClass->Properties.Volume = Value;
}

void UMainMenuWidget::Quit()
{
	if(!LoadTimer.IsValid())
	{
		GetWorld()->GetTimerManager().SetTimer(LoadTimer, this, &UMainMenuWidget::Quit, 1.0f, false);
		return;
	}
	GetWorld()->GetTimerManager().ClearTimer(LoadTimer);
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}

void UMainMenuWidget::SwitchToRadioButtons()
{
	int32 RadioIndex = 1;
	SwitchMenu(RadioIndex);
}

void UMainMenuWidget::AddRadioButtons()
{
	RadioButtonsWidget->AddValueToSize(1);
}

void UMainMenuWidget::RemoveRadioButtons()
{
	RadioButtonsWidget->AddValueToSize(-1);
}

void UMainMenuWidget::SwitchMenu(int32 WidgetIndex)
{
	MenuWidgetSwitcher->SetActiveWidgetIndex(WidgetIndex);
}


