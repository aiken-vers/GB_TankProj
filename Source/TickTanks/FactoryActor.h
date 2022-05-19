// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageTarget.h"
#include "HealthComponent.h"
#include "LevelPortal.h"
#include "TankPawn.h"
#include "Turret.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystem.h"
#include "Components/AudioComponent.h"
#include "FactoryActor.generated.h"

class UArrowComponent;
class UStaticMeshComponent;
class UBoxComponent;
class ALevelPortal;

UCLASS()
class TICKTANKS_API AFactoryActor : public AActor, public IDamageTarget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* Collision;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Building;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* SpawnPoint;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Effect")
	UAudioComponent* Audio_Death;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Effect")
	UParticleSystem* VisualEffect_Death;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Effect")
	UParticleSystem* VisualEffect_AfterDeath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	TSubclassOf<ATankPawn> SpawnTank;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	float SpawnRate = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	FName WaypointTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level")
	FName Team;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level")
	TArray<AActor*> FactoryWalls;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level")
	ALevelPortal* Portal;

	
	// Sets default values for this actor's properties
	AFactoryActor();

	virtual void TakeDamage(const FDamageInfo& DamageInfo) override;
	virtual void OnDeath();
	virtual void OnHealthChanged(float Health);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void SpawnTankTick();

public:	
	// Called every frame

private:
	FTimerHandle SpawnTimer;
	bool Destroyed = false;
};
