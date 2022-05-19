// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelPortal.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ALevelPortal::ALevelPortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>("Trigger");
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ALevelPortal::OnTargetBeginOverlap);
	RootComponent = Trigger;
	Body = CreateDefaultSubobject<UStaticMeshComponent>("Body");
	Body->SetupAttachment(RootComponent);
	InactiveLight = CreateDefaultSubobject<USpotLightComponent>("InactiveLight");
	InactiveLight->SetupAttachment(RootComponent);
	ActiveLight = CreateDefaultSubobject<USpotLightComponent>("ActiveLight");
	ActiveLight->SetupAttachment(RootComponent);
	

}

void ALevelPortal::ActivatePortal()
{
	if(!IsActive)
	{
		IsActive = true;
		InactiveLight->SetVisibility(false);
		ActiveLight->SetVisibility(true);
	}		
}

void ALevelPortal::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if(!IsActive)
	{
		InactiveLight->SetVisibility(true);
		ActiveLight->SetVisibility(false);
	}
	else
	{
		InactiveLight->SetVisibility(false);
		ActiveLight->SetVisibility(true);
	}
	
}

// Called when the game starts or when spawned
void ALevelPortal::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALevelPortal::OnTargetBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Pawn = Cast<APawn>(Other);
	if(Pawn && !LevelName.IsNone() && IsActive)
	{
		if(Pawn->GetController()==GetWorld()->GetFirstPlayerController())
		{
			UGameplayStatics::OpenLevel(GetWorld(), LevelName);
		}
	}
}

// Called every frame


