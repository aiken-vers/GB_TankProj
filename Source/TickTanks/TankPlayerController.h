// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TargetController.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

class ATankPawn;

/**
 * 
 */
UCLASS()
class TICKTANKS_API ATankPlayerController : public APlayerController, public ITargetController
{
	GENERATED_BODY()

public:
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;

	virtual void Tick(float DeltaSeconds) override;
	virtual FVector GetTargetLocation() const override;
private:
	void OnMoveForward(float Scale);
	void OnRotateRight(float Scale);
	void OnFire();
	void OnFireAlt();
	void OnSwapWeapons();
	
	UPROPERTY()
	ATankPawn* TankPawn;
	
	FVector MousePosition;
};
