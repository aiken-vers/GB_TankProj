// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructibleWall.h"

// Sets default values
ADestructibleWall::ADestructibleWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	Collision = CreateDefaultSubobject<UBoxComponent>("Collision");
	RootComponent = Collision;
	Body = CreateDefaultSubobject<UStaticMeshComponent>("Wall");
	Body->SetupAttachment(RootComponent);
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
	HealthComponent->OnDeath.AddUObject(this, &ADestructibleWall::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &ADestructibleWall::OnHealthChanged);
}

void ADestructibleWall::TakeDamage(const FDamageInfo& DamageInfo)
{
	HealthComponent->TakeDamage(DamageInfo);
}

void ADestructibleWall::OnDeath()
{
	if(IsDestructible)
		Destroy();
}

void ADestructibleWall::OnHealthChanged(float Health)
{
	
}

// Called when the game starts or when spawned
void ADestructibleWall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame


