// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"

#include "DrawDebugHelpers.h"
#include "TankPawn.h"
#include "Waypoint.h"
#include "Kismet/GameplayStatics.h"
#include "Microsoft/AllowMicrosoftPlatformTypes.h"

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Waypoints.Empty();
	TankPawn = Cast<ATankPawn>(InPawn);
	
	if(!TankPawn)
		return;

	TArray<AActor*> WaypointActors;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AWaypoint::StaticClass(), TankPawn->WaypointTag, WaypointActors);
	
	WaypointActors.Sort([](const AActor& a, const AActor& b)
	{
		auto WPA = CastChecked<AWaypoint>(&a);
		auto WPB = CastChecked<AWaypoint>(&b);
		return WPA->Order < WPB->Order;
	});

	for(auto Waypoint : WaypointActors)
	{
		Waypoints.Add(Waypoint->GetActorLocation());		
	}
	
	for(int i = 1; i<Waypoints.Num(); i++)
	{
		DrawDebugLine(GetWorld(), Waypoints[i-1], Waypoints[i], FColor::Yellow, true);
	}
	if(Waypoints.Num()>1)
	{
		DrawDebugLine(GetWorld(), Waypoints[Waypoints.Num()-1], Waypoints[0], FColor::Yellow, true);
	}
	
	NextWaypoint=0;
}

void AEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(!TankPawn)
		return;

	auto BestTarget = TankPawn->GetBestTarget();
	if(IsValid(BestTarget))
	{
		auto TargetToShootRotation = UKismetMathLibrary::FindLookAtRotation(TankPawn->Head->GetComponentLocation(), BestTarget->GetActorLocation());
		auto TurretRotation =  TankPawn->GetTurretRotation();
		TargetToShootRotation.Pitch=0;
		TargetToShootRotation.Roll=0;
		TurretRotation.Pitch=0;
		TurretRotation.Roll = 0;

		if(TurretRotation.Equals(TargetToShootRotation, 10))
		{
			TankPawn->FireAI();
		}
	}
	
	if(Waypoints.Num()<=0)
		return;
	if(NextWaypoint>=Waypoints.Num())
		NextWaypoint=0;
	auto Waypoint = Waypoints[NextWaypoint];

	if(2500 < FVector::DistSquared2D(Waypoint, TankPawn->GetActorLocation()))
	{
		TankPawn->MoveForward(1.0);
	}
	else
	{
		NextWaypoint++;
	}

	auto TargetRotation = UKismetMathLibrary::FindLookAtRotation(TankPawn->GetActorLocation(), Waypoint);
	TargetRotation.Pitch=0;
	TargetRotation.Roll=0;
	auto Rotation = GetControlRotation();
	Rotation.Pitch=0;
	Rotation.Roll=0;
	auto Direction = FRotator::NormalizeAxis(TargetRotation.Yaw - Rotation.Yaw);
	
	if(!TargetRotation.Equals(Rotation, 10))
	{
		if(!TargetRotation.Equals(Rotation, 90))
		{
			TankPawn->MoveForward(0);
		}
		TankPawn->RotateRight(Direction > 0 ? 1 : -1);
	}
	else
	{
		TankPawn->RotateRight(0);
	}
	
	//TankPawn->GetActorForwardVector();
	//auto Direction = TankPawn->GetActorLocation() - Waypoint;
	
	
}

FVector AEnemyAIController::GetTargetLocation() const
{
	if(TankPawn)
	{
		auto BestTarget =  TankPawn->GetBestTarget();
		if(BestTarget)
		{
			return BestTarget->GetTargetLocation();
		}
		return TankPawn->GetActorLocation() + TankPawn->GetActorForwardVector()*500;
	}
	return FVector::ZeroVector;
}


