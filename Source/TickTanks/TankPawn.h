// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefaultTankActor.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TankPlayerController.h"
#include "TankPawn.generated.h"

class UCameraComponent;

UCLASS()
class TICKTANKS_API ATankPawn : public ADefaultTankActor
{
	GENERATED_BODY()

public:	

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UCameraComponent* Camera;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Components")
	UParticleSystemComponent* VisualEffect_Damaged;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Components")
	UAudioComponent* Audio_AfterDeath;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MovementSpeed=100;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float RotationSpeed = 100;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float MovementAcceleration = 0.1f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float RotationAcceleration = 0.1f;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Armory")
	TSubclassOf<ACannon> SecondaryCannonClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Armory")
	int StartPrimaryAmmo = 40;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Armory")
	int StartSecondaryAmmo = 60;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	FName WaypointTag;

	int SpawnCounter = 0;
		

	// Sets default values for this pawn's properties	
	ATankPawn();	

	void MoveForward(float Scale);
	void RotateRight(float Scale);

	virtual void PossessedBy(AController* NewController) override;
	void Fire();
	void FireAI();
	void FireAlt();
	void ChangeCannon(TSubclassOf<ACannon> InCannonClass);
	void SwapWeapons();
	void RefillAmmo(float AmmoCap);
	void DeathCamera();

	virtual void OnConstruction(const FTransform& Transform) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	virtual void OnDeath() override;
	void OnDamaged();
	virtual void OnHealthChanged(float Health) override;
	float ForwardScaleMax=0;
	float ForwardScaleCurrent=0;
	float RotateScaleMax=0;
	float RotateScaleCurrent=0;
	TSubclassOf<ACannon> ActiveCannon;
	int PrimaryAmmo;
	int SecondaryAmmo;
	class ITargetController* TankController;

	FTimerHandle CameraTimer;
	FVector4 MonoSaturation;
	FVector4 MonoContrast;
};
