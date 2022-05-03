// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"

// Sets default values
ATurret::ATurret()
{
	PrimaryActorTick.bCanEverTick = true;
/*
	
	Collision = CreateDefaultSubobject<UBoxComponent>("Collision");
	RootComponent = Collision;
	
	Body = CreateDefaultSubobject<UStaticMeshComponent>("TurretBody");
	Body->SetupAttachment(RootComponent);
	Head = CreateDefaultSubobject<UStaticMeshComponent>("TurretHead");
	Head->SetupAttachment(Body);
	CannonSpawnPoint = CreateDefaultSubobject<UArrowComponent>("CannonSpawnPoint");
	CannonSpawnPoint->SetupAttachment(Head);
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
	HealthComponent->OnDeath.AddUObject(this, &ADefaultTankActor::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &ADefaultTankActor::OnHealthChanged);

*/
		
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	SetupCannon(CannonClass);	
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(BestTarget.IsValid())
	{		
		auto TurretRotation =  GetTurretRotation();
		auto Rotation = UKismetMathLibrary::FindLookAtRotation(Head->GetComponentLocation(), BestTarget->GetActorLocation());
		RotateHead(TurretRotation, Rotation, false);
		

		if(TurretRotation.Equals(Rotation, 2))
		{
			if(Cannon)
				Cannon->Fire();
		}
	}
}

void ATurret::OnHealthChanged(float Health)
{
	Super::OnHealthChanged(Health);
	GEngine->AddOnScreenDebugMessage(98768, 3, FColor::Purple, FString::Printf(TEXT("Turret HP %f"), Health));
}