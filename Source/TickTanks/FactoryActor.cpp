// Fill out your copyright notice in the Description page of Project Settings.


#include "FactoryActor.h"

#include "Interfaces/IHttpResponse.h"

// Sets default values
AFactoryActor::AFactoryActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Collision = CreateDefaultSubobject<UBoxComponent>("Collision");
	RootComponent = Collision;
	Building = CreateDefaultSubobject<UStaticMeshComponent>("Building");
	Building->SetupAttachment(RootComponent);
	SpawnPoint = CreateDefaultSubobject<UArrowComponent>("SpawnPoint");
	SpawnPoint->SetupAttachment(Building);
	Audio_Death = CreateDefaultSubobject<UAudioComponent>("Audio_Death");
	Audio_Death->SetupAttachment(RootComponent);
	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
	HealthComponent->OnDeath.AddUObject(this, &AFactoryActor::OnDeath);
}

void AFactoryActor::TakeDamage(const FDamageInfo& DamageInfo)
{
	HealthComponent->TakeDamage(DamageInfo);
}

void AFactoryActor::OnDeath()
{
	if(Destroyed)
		return;
	
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);
	
	if(Audio_Death)
	{
		if(Audio_Death)
			Audio_Death->Play();
	}
	if(VisualEffect_Death)
	{
		float ExplosionScale;
		if(GetActorScale().X < 1.0f)
			ExplosionScale = 2.0f/this->GetActorScale().X;
		else
			ExplosionScale = 2.0f*this->GetActorScale().X;
			
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), VisualEffect_Death, this->Building->GetComponentLocation(),
			FRotator::ZeroRotator, this->GetActorScale()*ExplosionScale);
		
		if(VisualEffect_AfterDeath)			
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), VisualEffect_AfterDeath, this->Building->GetComponentLocation(),
				FRotator::ZeroRotator, this->GetActorScale()*ExplosionScale);
	}
	for(AActor* Wall : FactoryWalls)
		Wall->Destroy();

	if(Portal)
		Portal->ActivatePortal();
	
	Destroyed = true;
	//Destroy();
}

void AFactoryActor::OnHealthChanged(float Health)
{
	
}

// Called when the game starts or when spawned
void AFactoryActor::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &AFactoryActor::SpawnTankTick, SpawnRate, true);
}

void AFactoryActor::SpawnTankTick()
{
	if(SpawnTank)
	{
		auto Transform = SpawnPoint->GetComponentTransform();
		auto Tank = GetWorld()->SpawnActorDeferred<ATankPawn>(SpawnTank, Transform,
			nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		static int SpawnCounter = 10;
		Tank->SpawnCounter = SpawnCounter;
		SpawnCounter++;
		if(!Team.IsNone())
			Tank->Team = Team;
		if(!WaypointTag.IsNone())
			Tank->WaypointTag = WaypointTag;
		
		UGameplayStatics::FinishSpawningActor(Tank, Transform);
	}
}

// Called every frame


