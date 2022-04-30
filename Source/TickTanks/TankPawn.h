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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
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
	int StartPrimaryAmmo = 20;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Armory")
	int StartSecondaryAmmo = 40;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	FName WaypointTag;
		

	// Sets default values for this pawn's properties	
	ATankPawn();	

	void MoveForward(float Scale);
	void RotateRight(float Scale);

	virtual void PossessedBy(AController* NewController) override;
	void Fire();
	void FireAlt();
	void ChangeCannon(TSubclassOf<ACannon> InCannonClass);
	void SwapWeapons();
	void RefillAmmo(float AmmoCap);
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
	virtual void OnHealthChanged(float Health) override;
	float ForwardScaleMax=0;
	float ForwardScaleCurrent=0;
	float RotateScaleMax=0;
	float RotateScaleCurrent=0;
	TSubclassOf<ACannon> ActiveCannon;
	int PrimaryAmmo = StartPrimaryAmmo;
	int SecondaryAmmo = StartSecondaryAmmo;
	class ATankPlayerController* TankController;
};
