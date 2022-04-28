// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"

#include "TankPlayerController.h"
#include "TickTanks.h"

#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

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

	CannonSpawnPoint = CreateDefaultSubobject<UArrowComponent>("CannonSpawnPoint");
	CannonSpawnPoint->SetupAttachment(TankTurret);
}

void ATankPawn::MoveForward(float Scale)
{
	ForwardScaleMax = Scale;	
}

void ATankPawn::RotateRight(float Scale)
{
	RotateScaleMax = Scale;	
}

void ATankPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	TankController = Cast<ATankPlayerController>(NewController);
	
}

void ATankPawn::Fire()
{
	if(Cannon && Cannon->bReadyToFire)
	{		
		if(ActiveCannon==CannonClass)
		{
			if(PrimaryAmmo<=0)
				return;
			
			Cannon->Fire();
			PrimaryAmmo--;			
		}			
		else
		{
			if(SecondaryAmmo<=0)
				return;
			
			Cannon->Fire();
			SecondaryAmmo--;
		}
		
	}
}

void ATankPawn::FireAlt()
{
	if(Cannon && Cannon->bReadyToFire)
	{		
		if(ActiveCannon==CannonClass)
		{
			if(PrimaryAmmo<Cannon->AltFireBurst)
				return;
			
			Cannon->FireAlt();
			PrimaryAmmo-=Cannon->AltFireBurst;			
		}			
		else
		{
			if(SecondaryAmmo<Cannon->AltFireBurst)
				return;
			
			Cannon->FireAlt();
			SecondaryAmmo-=Cannon->AltFireBurst;
		}		
	}	
}

void ATankPawn::SetupCannon(TSubclassOf<ACannon> InCannonClass)
{
	if(Cannon)
	{
		Cannon->Destroy();
		Cannon = nullptr;
	}
	CannonClass = InCannonClass;
	ActiveCannon = CannonClass;
	if(CannonClass)
	{
		auto Transform = CannonSpawnPoint->GetComponentTransform();
		FActorSpawnParameters Params;
		Params.Instigator = this;
		Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, Transform, Params);	
		Cannon->AttachToComponent(CannonSpawnPoint, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
}

void ATankPawn::SwapWeapons()
{
	if(ActiveCannon==CannonClass)
		ActiveCannon = SecondaryCannonClass;
	else
		ActiveCannon = CannonClass;	

	if(Cannon)
	{
		Cannon->Destroy();
		Cannon = nullptr;
	}
	if(ActiveCannon)
	{
		auto Transform = CannonSpawnPoint->GetComponentTransform();
		FActorSpawnParameters Params;
		Params.Instigator = this;
		Cannon = GetWorld()->SpawnActor<ACannon>(ActiveCannon, Transform, Params);	
		Cannon->AttachToComponent(CannonSpawnPoint, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
}

void ATankPawn::RefillAmmo(float AmmoCap)
{
	PrimaryAmmo = PrimaryAmmo + StartPrimaryAmmo*AmmoCap;
	SecondaryAmmo = SecondaryAmmo + StartSecondaryAmmo*AmmoCap;
}

void ATankPawn::TakeDamage(const FDamageInfo& DamageInfo)
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("Damage Taken %f"), DamageInfo.Damage));
}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	ActiveCannon = CannonClass;	
	SetupCannon(ActiveCannon);
}

void ATankPawn::Destroyed()
{
	Super::Destroyed();
	if(Cannon)
		Cannon->Destroy();
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ForwardScaleCurrent = FMath::Lerp(ForwardScaleCurrent, ForwardScaleMax, MovementAcceleration);
	RotateScaleCurrent = FMath::Lerp(RotateScaleCurrent, RotateScaleMax, RotationAcceleration);

	//Debug---------------
	
	//UE_LOG(LogTanks, Warning, TEXT("Forward scale = %f"), ForwardScaleCurrent);
	/*
	 *GEngine->AddOnScreenDebugMessage(2431, 0.1f, FColor::Red,
		FString::Printf(TEXT("Forward scale = %f"),ForwardScaleCurrent));
		*/

	if(ActiveCannon==CannonClass)
		GEngine->AddOnScreenDebugMessage(98755, -1, FColor::Blue, FString::Printf(TEXT("PRIM_AMMO %i"), PrimaryAmmo));
	else
		GEngine->AddOnScreenDebugMessage(98756, -1, FColor::Blue, FString::Printf(TEXT("SEC_AMMO %i"), SecondaryAmmo));
		
	//Debug---------------
	
	auto Location = GetActorLocation();
	auto ForwardVector =  GetActorForwardVector();
	SetActorLocation(Location+ForwardVector *ForwardScaleCurrent*MovementSpeed*DeltaTime, true);

	auto Rotation = GetActorRotation();
	Rotation.Yaw = (Rotation.Yaw + RotationSpeed * RotateScaleCurrent * DeltaTime);
	SetActorRotation(Rotation);

	if(TankController)
	{
		auto MousePosition = TankController->GetMousePosition();
		auto TurretRotation =  TankTurret->GetComponentRotation();
		FRotator MouseRotation = UKismetMathLibrary::FindLookAtRotation(TankTurret->GetComponentLocation(), MousePosition);
		MouseRotation.Roll = 0;
		MouseRotation.Pitch = 0;
		TankTurret->SetWorldRotation(FMath::Lerp(TurretRotation, MouseRotation, 0.1f));
	}	
}

// Called to bind functionality to input
void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

