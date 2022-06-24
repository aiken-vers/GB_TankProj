// Fill out your copyright notice in the Description page of Project Settings.


#include "FRadioButtonWidgetStyle.h"

FFRadioButtonStyle::FFRadioButtonStyle()
{
	CheckBoxStyle = FCoreStyle::Get().GetWidgetStyle<FCheckBoxStyle>("CheckBox");
	TextStyle = FCoreStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText");
}

FFRadioButtonStyle::~FFRadioButtonStyle()
{
}

const FName FFRadioButtonStyle::TypeName(TEXT("FFRadioButtonStyle"));

const FFRadioButtonStyle& FFRadioButtonStyle::GetDefault()
{
	static FFRadioButtonStyle Default;
	return Default;
}

void FFRadioButtonStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	// Add any brush resources here so that Slate can correctly atlas and reference them
}

