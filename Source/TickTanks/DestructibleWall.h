// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageTarget.h"
#include "HealthComponent.h"
#include "Components/BoxComponent.h"
#include "DestructibleWall.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class TICKTANKS_API ADestructibleWall : public AActor, public IDamageTarget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* Collision;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Body;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	bool IsDestructible=true;
	// Sets default values for this actor's properties
	ADestructibleWall();

	virtual void TakeDamage(const FDamageInfo& DamageInfo) override;
	virtual void OnDeath();
	virtual void OnHealthChanged(float Health);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame

};
