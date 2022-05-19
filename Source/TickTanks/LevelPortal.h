// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/SpotLightComponent.h"
#include "LevelPortal.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class TICKTANKS_API ALevelPortal : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* Trigger;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Body;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USpotLightComponent* InactiveLight;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USpotLightComponent* ActiveLight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level")
	bool IsActive = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level")
	FName LevelName;
	// Sets default values for this actor's properties
	ALevelPortal();

	void ActivatePortal();
	virtual void OnConstruction(const FTransform& Transform) override;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTargetBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	

public:	
	// Called every frame
	

};
