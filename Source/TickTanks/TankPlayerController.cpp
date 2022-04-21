// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"

#include "TankPawn.h"


void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("Forward", this, &ATankPlayerController::OnMoveForward);
	InputComponent->BindAxis("Right", this, &ATankPlayerController::OnRotateRight);
	
}

void ATankPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	TankPawn = Cast<ATankPawn>(GetPawn());
}

void ATankPlayerController::OnMoveForward(float Scale)
{
	if(TankPawn)
		TankPawn->MoveForward(Scale);
}

void ATankPlayerController::OnRotateRight(float Scale)
{
	if(TankPawn)
		TankPawn->RotateRight(Scale);
}
