// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"


#include "TickTanks.h"



// Sets default values
ATankPawn::ATankPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
	
	HealthComponent->OnDeath.AddUObject(this, &ATankPawn::OnDeath);
	HealthComponent->OnDamaged.AddUObject(this, &ATankPawn::OnDamaged);
	HealthComponent->OnHealthChanged.AddUObject(this, &ATankPawn::OnHealthChanged);

	VisualEffect_Damaged = CreateDefaultSubobject<UParticleSystemComponent>("VisualEffect_Damaged");
	VisualEffect_Damaged->SetupAttachment(RootComponent);

	PrimaryAmmo = StartPrimaryAmmo;
	SecondaryAmmo = StartSecondaryAmmo;
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
	TankController = Cast<ITargetController>(NewController);
	
}

void ATankPawn::Fire()
{
	if(Cannon && Cannon->bReadyToFire)
	{		
		if(ActiveCannon==CannonClass)
		{
			if(PrimaryAmmo<=0)
				return;
			
			PrimaryAmmo--;			
		}			
		else
		{
			if(SecondaryAmmo<=0)
				return;
			
			SecondaryAmmo--;
		}
		Cannon->Fire();
	}

	if(ActiveCannon==CannonClass)
		GEngine->AddOnScreenDebugMessage(98755, 60, FColor::Blue, FString::Printf(TEXT("PRIM_AMMO %i"), PrimaryAmmo));
	else
		GEngine->AddOnScreenDebugMessage(98756, 60, FColor::Blue, FString::Printf(TEXT("SEC_AMMO %i"), SecondaryAmmo));
}
void ATankPawn::FireAI()
{
	if(Cannon)
	{
		Cannon->Fire();
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
	if(ActiveCannon==CannonClass)
		GEngine->AddOnScreenDebugMessage(98755, 60, FColor::Blue, FString::Printf(TEXT("PRIM_AMMO %i"), PrimaryAmmo));
	else
		GEngine->AddOnScreenDebugMessage(98756, 60, FColor::Blue, FString::Printf(TEXT("SEC_AMMO %i"), SecondaryAmmo));
}

void ATankPawn::ChangeCannon(TSubclassOf<ACannon> InCannonClass)
{	
	CannonClass = InCannonClass;
	ActiveCannon = CannonClass;
	SetupCannon(ActiveCannon);
}

void ATankPawn::SwapWeapons()
{
	if(ActiveCannon==CannonClass)
		ActiveCannon = SecondaryCannonClass;
	else
		ActiveCannon = CannonClass;	

	SetupCannon(ActiveCannon);
}



void ATankPawn::RefillAmmo(float AmmoCap)
{
	PrimaryAmmo = PrimaryAmmo + StartPrimaryAmmo*AmmoCap;
	SecondaryAmmo = SecondaryAmmo + StartSecondaryAmmo*AmmoCap;
}

void ATankPawn::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	UE_LOG(LogTemp, Error, TEXT("Spawned %d"), SpawnCounter);
}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	ActiveCannon = CannonClass;	
	SetupCannon(ActiveCannon);
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
	/*
	if(ActiveCannon==CannonClass)
		GEngine->AddOnScreenDebugMessage(98755, -1, FColor::Blue, FString::Printf(TEXT("PRIM_AMMO %i"), PrimaryAmmo));
	else
		GEngine->AddOnScreenDebugMessage(98756, -1, FColor::Blue, FString::Printf(TEXT("SEC_AMMO %i"), SecondaryAmmo));
		*/
	//Debug---------------
	
	auto Location = GetActorLocation();
	auto ForwardVector =  GetActorForwardVector();
	SetActorLocation(Location+ForwardVector *ForwardScaleCurrent*MovementSpeed*DeltaTime, true);

	auto Rotation = GetActorRotation();
	Rotation.Yaw = (Rotation.Yaw + RotationSpeed * RotateScaleCurrent * DeltaTime);
	SetActorRotation(Rotation);

	if(TankController)
	{		
		auto TargetLocation = TankController->GetTargetLocation();
		RotateHead(TargetLocation, true);
	}	
}

// Called to bind functionality to input
void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATankPawn::OnDeath()
{	
	Super::OnDeath();	
	//UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}
void ATankPawn::OnDamaged()
{	
	if(VisualEffect_Damaged)
		VisualEffect_Damaged->Activate();
}

void ATankPawn::OnHealthChanged(float Health)
{
	GEngine->AddOnScreenDebugMessage(98757, 10000, FColor::Red, FString::Printf(TEXT("Tank HP %f"), Health));
}

