// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DefaultTankActor.h"
#include "Turret.generated.h"


UCLASS()
class TICKTANKS_API ATurret : public ADefaultTankActor
{
	GENERATED_BODY()
	
public:
	
	
	
	
	// Sets default values for this actor's properties
	ATurret();	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnHealthChanged(float Health) override;

	

private:
	bool TraceTarget();	
};


