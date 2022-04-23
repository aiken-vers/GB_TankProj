// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"


#include <thread>



// Sets default values
ACannon::ACannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	EmptyRoot = CreateDefaultSubobject<USceneComponent>("RootComponent");
	RootComponent = EmptyRoot;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	

	SpawnPoint = CreateDefaultSubobject<UArrowComponent>("SpawnPoint");
	SpawnPoint->SetupAttachment(RootComponent);
}

void ACannon::Fire()
{
	if(!bReadyToFire||Ammo<=0)
		return;
	
	switch(Type) {
		case ECannonType::Projectile:
			{
				GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("Projectile")));

				if(ProjectileType)
				{
					FActorSpawnParameters SpawnParams;
					SpawnParams.Instigator = GetInstigator();
					SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
					GetWorld()->SpawnActor<AProjectile>(ProjectileType, SpawnPoint->GetComponentTransform(), SpawnParams);
				}	
			}
			break;	
		case ECannonType::Trace:
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("Trace")));
			break;
	}
	Ammo--;
	bReadyToFire = false;
	GetWorld()->GetTimerManager().SetTimer(ReloadHandle, this, &ACannon::OnReload, FireRate, false);
}
void ACannon::FireAlt()
{
	if(!bReadyToFire||Ammo<AltFireBurst)
		return;

	for(int i=0;i<AltFireBurst;i++)
	{
		switch(Type) {
		case ECannonType::Projectile:
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("AltProjectile")));
			break;

		case ECannonType::Trace:
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("AltTrace")));
			break;
		}
	}
	Ammo-=AltFireBurst;
	bReadyToFire = false;
	GetWorld()->GetTimerManager().SetTimer(ReloadHandle, this, &ACannon::OnReload, FireRate, false);	
}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto RemainingTime = GetWorld()->GetTimerManager().GetTimerRemaining(ReloadHandle);
	GEngine->AddOnScreenDebugMessage(98754, -1, FColor::Blue, FString::Printf(TEXT("Until reload %f"), RemainingTime));
	GEngine->AddOnScreenDebugMessage(98755, -1, FColor::Blue, FString::Printf(TEXT("AMMO %i"), Ammo));

}

void ACannon::OnReload()
{
	bReadyToFire = true;
}



