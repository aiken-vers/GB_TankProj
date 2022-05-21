// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "Cannon.h"


#include <thread>

#include "DrawDebugHelpers.h"
#include "NavigationSystemTypes.h"
#include "Kismet/GameplayStatics.h"


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

	AudioComponent = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
	AudioComponent->SetupAttachment(RootComponent);
}

void ACannon::Fire()
{
	if(!bReadyToFire)
		return;
	
	FireEffect();
	
	switch(Type) {
		case ECannonType::Projectile:
			{
				//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("Projectile")));

				FireProjectile();
					
			}
			break;	
		case ECannonType::Trace:
			{
				FireTrace();
				
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
			GetWorld()->GetTimerManager().SetTimer(FireAltHandle,this, &ACannon::AltFire, AltFireRate, true);			
		}
		break;	
	case ECannonType::Trace:
		FireTraceAlt();
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

void ACannon::FireEffect()
{
	if(AudioComponent)
		AudioComponent->Play();
	if(BoomEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(BoomEffect, this->SpawnPoint, "GUN");
		/*
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BoomEffect, this->SpawnPoint->GetComponentLocation(),
			FRotator::ZeroRotator, this->GetActorScale());
			*/
		//VisualEffect->Activate();
	}
		
	if(CameraShakeEffect)
	{
		if(this->GetInstigator()->GetInstigatorController()==GetWorld()->GetFirstPlayerController())
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(CameraShakeEffect);
	}
	//*************VIBRATION****************
	if(ForceFeedbackEffect)
	{		
		FForceFeedbackParameters FeedParams;
		FeedParams.bLooping=false;
		if(this->GetInstigator()->GetInstigatorController()==GetWorld()->GetFirstPlayerController())
			GetWorld()->GetFirstPlayerController()->ClientPlayForceFeedback(ForceFeedbackEffect, FeedParams);
	}
	//*************VIBRATION****************
	
}

void ACannon::FireProjectile()
{
	if(ProjectileType)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = GetInstigator();
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<ABaseProjectile>(ProjectileType, SpawnPoint->GetComponentTransform(), SpawnParams);
	}
}

void ACannon::FireTrace()
{
	FHitResult TraceEnd;
	TArray<FHitResult> HitArray;
    auto Start = SpawnPoint-> GetComponentLocation();
    auto End = Start+SpawnPoint->GetForwardVector()*2000;

	TraceEnd = TraceToHit(Start, End);
	if(TraceEnd.IsValidBlockingHit())
		End = TraceEnd.Location;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetInstigator());
	
	const bool Hit = UKismetSystemLibrary::SphereTraceMulti(GetWorld(),
		Start, End, 50.0f,
		UEngineTypes::ConvertToTraceType(ECC_Camera), false, ActorsToIgnore,
		EDrawDebugTrace::ForDuration, HitArray, true,
		FLinearColor::Blue, FLinearColor::Gray, 2.0f);

	if(Hit)
	{
		for(const FHitResult SphereHit : HitArray)
		{
			if(SphereHit.Actor.IsValid())
			{
				//HitResult.Actor->Destroy();
				auto DamageTarget =  Cast<IDamageTarget>(SphereHit.Actor);
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
	}	
}

void ACannon::FireTraceAlt()
{
	FireEffect();
	
	FHitResult HitResult;
    auto Start = SpawnPoint-> GetComponentLocation();
    auto End = Start+SpawnPoint->GetForwardVector()*2000;    

	HitResult = TraceToHit(Start, End);
	
    if(HitResult.IsValidBlockingHit())
    {
    	DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Cyan, false, 2);
    	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("Trace")));
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
}

FHitResult ACannon::TraceToHit(FVector Start, FVector End)
{
	FHitResult TracedHit;
	
	FCollisionObjectQueryParams ObjQuerryParams;
	ObjQuerryParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjQuerryParams.AddObjectTypesToQuery(ECC_Vehicle);
	ObjQuerryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjQuerryParams.AddObjectTypesToQuery(ECC_Destructible);
	ObjQuerryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetInstigator());

	GetWorld()->LineTraceSingleByObjectType(TracedHit, Start, End, ObjQuerryParams,Params);

	return TracedHit;	
}

void ACannon::AltFire()
{
	if(BurstFired>=AltFireBurst)
	{
		StopAltFire();
		return;
	}		
	FireEffect();
	FireProjectile();
	BurstFired++;
}

void ACannon::StopAltFire()
{
	GetWorld()->GetTimerManager().ClearTimer(FireAltHandle);
	BurstFired=0;	
}



