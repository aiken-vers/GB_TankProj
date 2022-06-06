// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultTankActor.h"




// Sets default values

ADefaultTankActor::ADefaultTankActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.	

	Collision = CreateDefaultSubobject<UBoxComponent>("Collision");
	RootComponent = Collision;
	
	Body = CreateDefaultSubobject<UStaticMeshComponent>("TurretBody");
	Body->SetupAttachment(RootComponent);
	Head = CreateDefaultSubobject<UStaticMeshComponent>("TurretHead");
	Head->SetupAttachment(Body);
	CannonSpawnPoint = CreateDefaultSubobject<UArrowComponent>("CannonSpawnPoint");
	CannonSpawnPoint->SetupAttachment(Head);

	TargetRange=CreateDefaultSubobject<USphereComponent>("TargetRange");
	TargetRange->SetupAttachment(RootComponent);
	TargetRange->OnComponentBeginOverlap.AddDynamic(this, &ADefaultTankActor::OnTargetBeginOverlap);
	TargetRange->OnComponentEndOverlap.AddDynamic(this, &ADefaultTankActor::OnTargetEndOverlap);
	
	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
	HealthComponent->OnDeath.AddUObject(this, &ADefaultTankActor::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &ADefaultTankActor::OnHealthChanged);

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(RootComponent);

	Audio_Death = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
	Audio_Death->SetupAttachment(RootComponent);
}


void ADefaultTankActor::TakeDamage(const FDamageInfo& DamageInfo)
{
	if(VisualEffect_Death)
	{
		UGameplayStatics::SpawnEmitterAttached(VisualEffect_Death, this->Body, "BODY");
	}
	HealthComponent->TakeDamage(DamageInfo);	
}


void ADefaultTankActor::RotateHead(FVector Target, bool Rotate2D)
{
	auto TargetRotation = UKismetMathLibrary::FindLookAtRotation(Head->GetComponentLocation(), Target);
	auto HeadRotation =  Head->GetComponentRotation();
	if(Rotate2D)
	{
		HeadRotation.Roll = 0;
		HeadRotation.Pitch = 0;
	}
	Head->SetWorldRotation(FMath::Lerp(HeadRotation, TargetRotation, 0.1f));
}
void ADefaultTankActor::RotateHead(FRotator HeadRotation, FRotator TargetRotation, bool Rotate2D)
{
	if(Rotate2D)
	{
		HeadRotation.Roll = 0;
		HeadRotation.Pitch = 0;
	}
	Head->SetWorldRotation(FMath::Lerp(HeadRotation, TargetRotation, 0.1f));
}

void ADefaultTankActor::SetupCannon(TSubclassOf<ACannon> InCannonClass)
{
	if(Cannon)
	{
		Cannon->Destroy();
		Cannon = nullptr;
	}
	if(InCannonClass)
	{
		auto Transform = CannonSpawnPoint->GetComponentTransform();
		FActorSpawnParameters Params;
		Params.Instigator = this;
		Cannon = GetWorld()->SpawnActor<ACannon>(InCannonClass, Transform, Params);	
		Cannon->AttachToComponent(CannonSpawnPoint, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
}

// Called when the game starts or when spawned

void ADefaultTankActor::Destroyed()
{
	Super::Destroyed();
	if(Cannon)
		Cannon->Destroy();
}

void ADefaultTankActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(HealthBar)
	{
		auto CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
		
		auto TargetRotation = UKismetMathLibrary::FindLookAtRotation(
			HealthBar->GetComponentLocation(),
			CameraManager->GetCameraLocation()
			);
		
		HealthBar->SetWorldRotation(TargetRotation);
	}
}

// Called every frame


void ADefaultTankActor::OnDeath()
{
	if(AfterDeath)
		return;
	
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
			
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), VisualEffect_Death, this->Body->GetComponentLocation(),
			FRotator::ZeroRotator, this->GetActorScale()*ExplosionScale);
	}
	if(this->GetController()==GetWorld()->GetFirstPlayerController())
	{
		AfterDeath = true;
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		PlayerController->SetInputMode(FInputModeUIOnly());		
		this->SetActorTickEnabled(false);
		Collision->Deactivate();
		if(VisualEffect_AfterDeath)			
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), VisualEffect_AfterDeath, this->Body->GetComponentLocation(),
				FRotator::ZeroRotator, this->GetActorScale());
		return;
	}
	Destroy();
}

void ADefaultTankActor::OnHealthChanged(float Health)
{
	UpdateHealth();
}

void ADefaultTankActor::UpdateHealth()
{
	float HealthPercent =  HealthComponent->GetHealth() / HealthComponent->MaxHealth;
	
	UHealthWidget* HealthWidget = Cast<UHealthWidget>(HealthBar->GetUserWidgetObject());
	HealthWidget->SetHP(HealthPercent);	
}

void ADefaultTankActor::BeginPlay()
{
	Super::BeginPlay();

	if(HealthBar)
	{
		HealthBar->SetVisibility(ShowHealthBar);
		UpdateHealth();
	}	
}

//***********TARGETING**************************
void ADefaultTankActor::OnTargetBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
								   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Error, TEXT("Overlaped with %s"), *Other->GetName());
	if(ADefaultTankActor* Target = Cast<ADefaultTankActor>(Other))
	{
		UE_LOG(LogTemp, Error, TEXT("Overlaped: Other->ATankActor"));
		if(Other==this)
		{
			UE_LOG(LogTemp, Error, TEXT("Overlaped: Other==this"));
			return;
		}
			
		if(!Target->Team.IsNone() && Target->Team==this->Team)
			return;
		Targets.Add(Other);
		if(!BestTarget.IsValid())
		{
			FindBestTarget();
		}
	}
}

void ADefaultTankActor::OnTargetEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	
	if(Other==this)
		return;	
		
	Targets.Remove(Other);
	if(Other == BestTarget)
	{
		FindBestTarget();
	}
}

void ADefaultTankActor::FindBestTarget()
{
	float MinDistance = 5000000;
	BestTarget = nullptr;
	for(auto Target : Targets)
	{
		auto Distance = FVector::Dist2D(GetActorLocation(), Target->GetActorLocation());
		if(MinDistance > Distance)
		{
			MinDistance = Distance;
			BestTarget = Target;
		}
	}
}
//***********TARGETING**************************
