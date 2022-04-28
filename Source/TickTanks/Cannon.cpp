// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"


#include <thread>

#include "DrawDebugHelpers.h"


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
	if(!bReadyToFire)
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
			{
				FHitResult HitResult;
				auto Start = SpawnPoint-> GetComponentLocation();
				auto End = Start+SpawnPoint->GetForwardVector()*2000;
				FCollisionObjectQueryParams ObjQuerryParams;
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
					DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Cyan, false, 2);
					GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("Trace")));
					if(HitResult.Actor.IsValid())
					{
						//HitResult.Actor->Destroy();
						auto DamageTarget =  Cast<IDamageTarget>(HitResult.Actor);
						if(DamageTarget)
						{
							FDamageInfo DInfo;
							DInfo.Damage = TraceDamage;
							DInfo.Instigator = GetInstigator();
							DInfo.Attacker = this;
							DamageTarget->TakeDamage(DInfo);
						}
					}
					
				}
				else
				{
					DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 2);
				}
				
				break;
			}
			
	}
	bReadyToFire = false;
	GetWorld()->GetTimerManager().SetTimer(ReloadHandle, this, &ACannon::OnReload, FireRate, false);
}
void ACannon::FireAlt()
{
	if(!bReadyToFire)
		return;

	switch(Type) {
	case ECannonType::Projectile:
		{
			for(int i=0;i<AltFireBurst;i++)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("AltProjectile")));

				if(ProjectileType)
				{
					FActorSpawnParameters SpawnParams;
					SpawnParams.Instigator = GetInstigator();
					SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
					GetWorld()->SpawnActor<AProjectile>(ProjectileType, SpawnPoint->GetComponentTransform(), SpawnParams);
				}				
			}				
		}
		break;	
	case ECannonType::Trace:
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("Trace")));
		break;
	}
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
}

void ACannon::OnReload()
{
	bReadyToFire = true;
}



