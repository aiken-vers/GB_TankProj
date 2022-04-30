// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Cannon.h"
#include "DamageTarget.h"
#include "HealthComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DefaultTankActor.generated.h"

class UArrowComponent;
class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class TICKTANKS_API ADefaultTankActor : public APawn, public IDamageTarget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* Collision;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Body;	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Head;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* CannonSpawnPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Armory")
	TSubclassOf<ACannon> CannonClass;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Armory")
	ACannon* Cannon;

	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;
	
	
	
	// Sets default values for this actor's properties
	ADefaultTankActor();

	
	virtual void TakeDamage(const FDamageInfo& DamageInfo) override;
	void RotateHead(FVector Target,bool Rotate2D);
	void RotateHead(FRotator HeadRotation, FRotator TargetRotation, bool Rotate2D);
	void SetupCannon(TSubclassOf<ACannon> InCannonClass);
	virtual void OnDeath();
	virtual void OnHealthChanged(float Health);

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;
	virtual void Destroyed() override;
	

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

private:
	

};
