// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "DamageTarget.h"
#include "BaseProjectile.generated.h"

UCLASS()
class TICKTANKS_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USphereComponent* Collision;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UMeshComponent* Mesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	float Damage = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	float ExplosionRadius = 100.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	float ExplosionImpulse = 1000.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	bool HasForce = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	bool IsExplosive = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	bool DamageFromExplosion = false;
	
	// Sets default values for this actor's properties
	ABaseProjectile();

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	bool NoSelfHit(AActor* OverlapedActor);
	void DoDamage(AActor* OverlapedActor);
	void MakeExplosion(FVector HitLocation, float Force=0.0);
	void UseForce(UPrimitiveComponent* Target, FVector ForceVector);
public:	
	// Called every frame

};
