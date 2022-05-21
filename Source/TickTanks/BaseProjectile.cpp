// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProjectile.h"

// Sets default values
ABaseProjectile::ABaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	Collision = CreateDefaultSubobject<USphereComponent>("Collision");
	RootComponent = Collision;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh -> SetupAttachment(RootComponent);
}


// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

bool ABaseProjectile::NoSelfHit(AActor* OverlapedActor)
{
	return OverlapedActor != GetInstigator() && OverlapedActor->GetInstigator() != GetInstigator();
}

void ABaseProjectile::DoDamage(AActor* OverlapedActor)
{
	auto DamageTarget =  Cast<IDamageTarget>(OverlapedActor);
	if(DamageTarget)
	{
		FDamageInfo DInfo;
		DInfo.Damage = Damage;
		DInfo.Instigator = GetInstigator();
		DInfo.Attacker = this;
		DamageTarget->TakeDamage(DInfo);
	}
}

void ABaseProjectile::MakeExplosion(FVector HitLocation, float Force)
{
	TArray<FHitResult> HitResults;
	
	if(GetWorld()->SweepMultiByChannel(HitResults, HitLocation, HitLocation, FQuat::Identity,
		ECollisionChannel::ECC_Visibility, FCollisionShape::MakeSphere(ExplosionRadius)))
	{
		for(auto HitResult : HitResults)
		{
			if(HitResult.Actor.IsValid())
			{				
				if(NoSelfHit(HitResult.Actor.Get()))
				{
					if(DamageFromExplosion)
						DoDamage(HitResult.Actor.Get());
				}
				if(HasForce)
				{
					UPrimitiveComponent* HitComponent = Cast<UPrimitiveComponent>(HitResult.GetComponent());
					FVector Vector = HitResult.ImpactPoint - GetActorLocation();
					Vector.Normalize();
					UseForce(HitComponent, Vector*Force);
				}				
			}
		}
	}
}

void ABaseProjectile::UseForce(UPrimitiveComponent* Target, FVector ForceVector)
{
	if(Target)
	{
		if(Target->IsSimulatingPhysics())
		{			
			Target->AddForce(ForceVector*20);
			Target->AddImpulse(ForceVector);						
		}
	}
}


