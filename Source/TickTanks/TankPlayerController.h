// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

class ATankPawn;

/**
 * 
 */
UCLASS()
class TICKTANKS_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
private:
	void OnMoveForward(float Scale);
	void OnRotateRight(float Scale);
	
	UPROPERTY()
	ATankPawn* TankPawn;
};
