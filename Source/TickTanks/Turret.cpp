// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"

#include "DrawDebugHelpers.h"

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
			if(TraceTarget())
			{
				if(Cannon)
					Cannon->Fire();
			}			
		}
	}
}

void ATurret::OnHealthChanged(float Health)
{
	Super::OnHealthChanged(Health);
	GEngine->AddOnScreenDebugMessage(98768, 3, FColor::Purple, FString::Printf(TEXT("Turret HP %f"), Health));
}

bool ATurret::TraceTarget()
{
	FHitResult HitResult;
	auto Range = TargetRange->GetScaledSphereRadius();
	auto Start = CannonSpawnPoint-> GetComponentLocation();
	auto End = Start+CannonSpawnPoint->GetForwardVector()*Range;
	FCollisionObjectQueryParams ObjQuerryParams;
	ObjQuerryParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjQuerryParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjQuerryParams.AddObjectTypesToQuery(ECC_Vehicle);
	ObjQuerryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjQuerryParams.AddObjectTypesToQuery(ECC_Destructible);
	ObjQuerryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetInstigator());
	
	if(GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, ObjQuerryParams,Params))
	{
		if(HitResult.Actor.IsValid() && Cast<APawn>(HitResult.Actor))
		{
			//DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Magenta, false, 1);
			return true;
		}
		//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1);
	}
	else
	{
		//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1);
	}
	return false;
}
