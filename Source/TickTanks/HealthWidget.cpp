// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthWidget.h"

void UHealthWidget::SetHP(float value)
{
	HealthBar->SetPercent(value);
}