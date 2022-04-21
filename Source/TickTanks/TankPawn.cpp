// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"

#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ATankPawn::ATankPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<UBoxComponent>("Collision");
	RootComponent = Collision;

	TankBody = CreateDefaultSubobject<UStaticMeshComponent>("TankBody");
	TankBody->SetupAttachment(RootComponent);
	TankTurret = CreateDefaultSubobject<UStaticMeshComponent>("TankTurret");
	TankTurret->SetupAttachment(TankBody);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
}

void ATankPawn::MoveForward(float Scale)
{
	ForwardScaleMax = Scale;	
}

void ATankPawn::RotateRight(float Scale)
{
	RotateScaleMax = Scale;	
}
// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ForwardScaleCurrent = FMath::Lerp(ForwardScaleCurrent, ForwardScaleMax, 0.1f);
	RotateScaleCurrent = FMath::Lerp(RotateScaleCurrent, RotateScaleMax, 0.1f);
	
	auto Location = GetActorLocation();
	auto ForwardVector =  GetActorForwardVector();
	SetActorLocation(Location+ForwardVector *ForwardScaleCurrent*MovementSpeed*DeltaTime, true);

	auto Rotation = GetActorRotation();
	Rotation.Yaw = (Rotation.Yaw + RotationSpeed * RotateScaleCurrent * DeltaTime);
	SetActorRotation(Rotation);
}

// Called to bind functionality to input
void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

