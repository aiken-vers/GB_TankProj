// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"

#include "DrawDebugHelpers.h"
#include "TankPawn.h"


void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("Forward", this, &ATankPlayerController::OnMoveForward);
	InputComponent->BindAxis("Right", this, &ATankPlayerController::OnRotateRight);
	InputComponent->BindAction("Shoot", IE_Pressed, this, &ATankPlayerController::OnFire);
	InputComponent->BindAction("ShootAlt", IE_Pressed, this, &ATankPlayerController::OnFireAlt);
	
	SetShowMouseCursor(true);
}

void ATankPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	TankPawn = Cast<ATankPawn>(GetPawn());
}

void ATankPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);	
	 
	FVector ScreenMousePosition;
	FVector MouseDirection;
	DeprojectMousePositionToWorld(ScreenMousePosition, MouseDirection);
	
	//DrawDebugLine(GetWorld(), ScreenMousePosition, ScreenMousePosition + MouseDirection*5000, FColor::Blue, false, 1, 0, 5);

	auto Z = FMath::Abs( GetPawn()->GetActorLocation().Z - ScreenMousePosition.Z);
	MousePosition = ScreenMousePosition - Z * MouseDirection / MouseDirection.Z;
	//DrawDebugSphere(GetWorld(), MousePosition, 20, 16, FColor::Red, false, 1);

	
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

void ATankPlayerController::OnFire()
{
	if(TankPawn)
		TankPawn->Fire();
}
void ATankPlayerController::OnFireAlt()
{
	if(TankPawn)
		TankPawn->FireAlt();
}