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
	if(Waypoints.Num()<=0)
		return;
	if(NextWaypoint>=Waypoints.Num())
		NextWaypoint=0;
	auto Waypoint = Waypoints[NextWaypoint];

	auto TargetRotation = UKismetMathLibrary::FindLookAtRotation(TankPawn->GetActorLocation(), Waypoint);
	TargetRotation.Pitch=0;
	TargetRotation.Roll=0;
	auto Rotation = GetControlRotation();
	Rotation.Pitch=0;
	Rotation.Roll=0;
	if(!TargetRotation.Equals(Rotation, 10))
	{
		TankPawn->RotateRight(1.0);
	}
	else
	{
		TankPawn->RotateRight(0);
	}
	if(2500 < FVector::DistSquared2D(Waypoint, TankPawn->GetActorLocation()))
	{
		TankPawn->MoveForward(1.0);
	}
	else
	{
		NextWaypoint++;
	}

	//TankPawn->GetActorForwardVector();
	//auto Direction = TankPawn->GetActorLocation() - Waypoint;
	
	
}


