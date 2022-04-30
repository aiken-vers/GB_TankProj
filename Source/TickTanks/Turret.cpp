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
	TargetRange=CreateDefaultSubobject<USphereComponent>("TargetRange");
	TargetRange->SetupAttachment(RootComponent);
	TargetRange->OnComponentBeginOverlap.AddDynamic(this, &ATurret::OnTargetBeginOverlap);
	TargetRange->OnComponentEndOverlap.AddDynamic(this, &ATurret::OnTargetEndOverlap);	
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
		auto Rotation = UKismetMathLibrary::FindLookAtRotation(Head->GetComponentLocation(), BestTarget->GetActorLocation());
		auto TurretRotation =  Head->GetComponentRotation();
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
	GEngine->AddOnScreenDebugMessage(98768, 10000, FColor::Purple, FString::Printf(TEXT("Turret HP %f"), Health));
}

void ATurret::OnTargetBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
                                   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(Other==this)
		return;
	Targets.Add(Other);
	if(!BestTarget.IsValid())
	{
		FindBestTarget();
	}
}

void ATurret::OnTargetEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if(Other==this)
		return;
	Targets.Remove(Other);
	if(Other == BestTarget)
	{
		FindBestTarget();
	}
}

void ATurret::FindBestTarget()
{
	float MinDistance = 5000000;
	BestTarget = nullptr;
	for(auto Target : Targets)
	{
		auto Distance = FVector::Dist2D(GetActorLocation(), Target->GetActorLocation());
		if(MinDistance > Distance)
		{
			MinDistance = Distance;
			BestTarget = Target;
		}
	}
}
