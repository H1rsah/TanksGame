// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "ProjectileMortair.generated.h"

/**
 * 
 */
UCLASS()
class TANKSGAME_API AProjectileMortair : public AProjectileBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UProjectileMovementComponent* ProjectileMovementComponent;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent * TrailEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float MoveAccurency = 10;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Trajectory")
	float SimulationMaxTime = 50;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Trajectory")
	float SimulationTimeStep = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Trajectory")
	float SimulationSpeed = 20;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Trajectory")
	bool ShowTrajectory = false;
	UPROPERTY(BlueprintReadWrite, Category = "Movement params")
	FVector MoveVector;
	UPROPERTY(BlueprintReadWrite, Category = "Movement params")
	TArray<FVector> CurrentTrajectory;
	UPROPERTY(BlueprintReadWrite, Category = "Movement params")
	int32 TragectoryPointIndex;
	
public:
	AProjectileMortair();
	
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& SweepResult) override;
	virtual void Start() override;

protected:
	virtual void Move(float DeltaTime) override;
};
