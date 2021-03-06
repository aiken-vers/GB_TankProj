// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TargetController.h"
#include "EnemyAIController.generated.h"

class ATankPawn;
/**
 * 
 */
UCLASS()
class TICKTANKS_API AEnemyAIController : public AAIController, public ITargetController
{
	GENERATED_BODY()

public:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual FVector GetTargetLocation() const override;
private:
	TArray<FVector> Waypoints;

	int NextWaypoint = 0;

	UPROPERTY()
	ATankPawn* TankPawn;
	
};
