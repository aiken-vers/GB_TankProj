// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<UBoxComponent>("Collision");
	RootComponent = Collision;

	TurretBody = CreateDefaultSubobject<UStaticMeshComponent>("TurretBody");
	TurretBody->SetupAttachment(RootComponent);
	TurretHead = CreateDefaultSubobject<UStaticMeshComponent>("TurretHead");
	TurretHead->SetupAttachment(TurretBody);
	CannonSpawnPoint = CreateDefaultSubobject<UArrowComponent>("CannonSpawnPoint");
	CannonSpawnPoint->SetupAttachment(TurretHead);
	
	TargetRange = CreateDefaultSubobject<USphereComponent>("TargetRange");
	TargetRange->SetupAttachment(RootComponent);
	TargetRange->OnComponentBeginOverlap.AddDynamic(this, &ATurret::OnTargetBeginOverlap);
	TargetRange->OnComponentEndOverlap.AddDynamic(this, &ATurret::OnTargetEndOverlap);
	

}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	if(CannonClass)
	{
		auto Transform = CannonSpawnPoint->GetComponentTransform();
		Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, Transform);	
		Cannon->AttachToComponent(CannonSpawnPoint, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
	
}

void ATurret::Destroyed()
{
	Super::Destroyed();
	
	if(Cannon)
		Cannon->Destroy();
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(BestTarget.IsValid())
	{
		auto Rotation = UKismetMathLibrary::FindLookAtRotation(TurretHead->GetComponentLocation(), BestTarget->GetActorLocation());
		auto TurretRotation =  TurretHead->GetComponentRotation();		
		//Rotation.Roll = 0;
		//Rotation.Pitch = 0;
		TurretHead->SetWorldRotation(FMath::Lerp(TurretRotation, Rotation, 0.1f));

		if(TurretRotation.Equals(Rotation, 2))
		{
			if(Cannon)
				Cannon->Fire();
		}
	}
	

	

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

