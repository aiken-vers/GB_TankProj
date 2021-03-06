// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoBox.h"

// Sets default values
AAmmoBox::AAmmoBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	Collision = CreateDefaultSubobject<UBoxComponent>("Collision");
    RootComponent = Collision;
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AAmmoBox::OnBeginOverlap);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh -> SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AAmmoBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAmmoBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAmmoBox::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Tank = Cast<ATankPawn>(Other);
	if(Tank)
	{
		if(CannonClass)
			Tank->ChangeCannon(CannonClass);
		
		Tank->RefillAmmo(AmmoCapasity);
		Destroy();
	}	
}

