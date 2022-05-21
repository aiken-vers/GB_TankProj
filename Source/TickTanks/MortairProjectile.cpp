// Fill out your copyright notice in the Description page of Project Settings.


#include "MortairProjectile.h"



// Sets default values
AMortairProjectile::AMortairProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");

	ParticleTail = CreateDefaultSubobject<UParticleSystemComponent>("ParticleTail");
	ParticleTail->SetupAttachment(RootComponent);

}

void AMortairProjectile::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	if(VisualEffect_Explosion)
	{
		float ExplosionScale;		
		ExplosionScale = ExplosionEffectScale*this->GetActorScale().X;	
			
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), VisualEffect_Explosion, HitLocation,
			FRotator::ZeroRotator, this->GetActorScale()*ExplosionScale);
	}
	
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	
	Destroy();
	MovementComponent->StopSimulating(Hit);

	TArray<FHitResult> HitResults;
	HitResults.Add(Hit);
	if(GetWorld()->SweepMultiByChannel(HitResults, HitLocation, HitLocation, FQuat::Identity,
		ECollisionChannel::ECC_Visibility, FCollisionShape::MakeSphere(ExplosionRadius)))
	{
		for(auto HitResult : HitResults)
		{
			if(HitResult.Actor.IsValid())
			{
				if(HitResult.Actor != GetInstigator() && HitResult.Actor->GetInstigator() != GetInstigator())
				{
					auto DamageTarget =  Cast<IDamageTarget>(HitResult.Actor);
					if(DamageTarget)
					{
						FDamageInfo DInfo;
						DInfo.Damage = Damage;
						DInfo.Instigator = GetInstigator();
						DInfo.Attacker = this;
						DamageTarget->TakeDamage(DInfo);
					}
				}				
				UPrimitiveComponent* HitComponent = Cast<UPrimitiveComponent>(HitResult.GetComponent());
				if(HitComponent)
				{
					if(HitComponent->IsSimulatingPhysics())
					{
						FVector Vector = HitResult.ImpactPoint - GetActorLocation();
						Vector.Normalize();
						HitResult.Component->AddImpulse(Vector*MovementComponent->InitialSpeed*ExplosionImpulse);
					}
				}
			}
		}
	}
	
}

// Called when the game starts or when spawned
void AMortairProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMortairProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

