// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultTankActor.h"



// Sets default values

ADefaultTankActor::ADefaultTankActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.	

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
	
}


void ADefaultTankActor::TakeDamage(const FDamageInfo& DamageInfo)
{
	HealthComponent->TakeDamage(DamageInfo);
}


void ADefaultTankActor::RotateHead(FVector Target, bool Rotate2D)
{
	auto TargetRotation = UKismetMathLibrary::FindLookAtRotation(Head->GetComponentLocation(), Target);
	auto HeadRotation =  Head->GetComponentRotation();
	if(Rotate2D)
	{
		HeadRotation.Roll = 0;
		HeadRotation.Pitch = 0;
	}
	Head->SetWorldRotation(FMath::Lerp(HeadRotation, TargetRotation, 0.1f));
}
void ADefaultTankActor::RotateHead(FRotator HeadRotation, FRotator TargetRotation, bool Rotate2D)
{
	if(Rotate2D)
	{
		HeadRotation.Roll = 0;
		HeadRotation.Pitch = 0;
	}
	Head->SetWorldRotation(FMath::Lerp(HeadRotation, TargetRotation, 0.1f));
}

void ADefaultTankActor::SetupCannon(TSubclassOf<ACannon> InCannonClass)
{
	if(Cannon)
	{
		Cannon->Destroy();
		Cannon = nullptr;
	}
	if(InCannonClass)
	{
		auto Transform = CannonSpawnPoint->GetComponentTransform();	
		Cannon = GetWorld()->SpawnActor<ACannon>(InCannonClass, Transform);	
		Cannon->AttachToComponent(CannonSpawnPoint, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
}

// Called when the game starts or when spawned

void ADefaultTankActor::Destroyed()
{
	Super::Destroyed();
	if(Cannon)
		Cannon->Destroy();
}

// Called every frame


void ADefaultTankActor::OnDeath()
{
	Destroy();
}

void ADefaultTankActor::OnHealthChanged(float Health)
{
	GEngine->AddOnScreenDebugMessage(98768, 10000, FColor::Purple, FString::Printf(TEXT("Tank HP %f"), Health));
}

