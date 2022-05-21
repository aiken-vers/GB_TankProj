// Fill out your copyright notice in the Description page of Project Settings.


#include "MortairProjectile.h"


// Sets default values
AMortairProjectile::AMortairProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");

	ParticleTail = CreateDefaultSubobject<UParticleSystemComponent>("ParticleTail");
	ParticleTail->SetupAttachment(RootComponent);

}

void AMortairProjectile::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	
	if(VisualEffect_Explosion)
	{
		float ExplosionScale;		
		ExplosionScale = ExplosionEffectScale*this->GetActorScale().X;	
			
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), VisualEffect_Explosion, HitLocation,
			FRotator::ZeroRotator, this->GetActorScale()*ExplosionScale);
	}
	Destroy();
	MovementComponent->StopSimulating(Hit);

	float Force = MovementComponent->InitialSpeed*ExplosionImpulse;	
	MakeExplosion(HitLocation, Force);
}

// Called when the game starts or when spawned
void AMortairProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame


