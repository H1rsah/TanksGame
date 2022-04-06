// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyTank.h"
#include "TankAIController.generated.h"

/**
 * 
 */
UCLASS()
class TANKSGAME_API ATankAIController : public AAIController
{
	GENERATED_BODY()
	
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	void GetClosestTarget();
	void GeneratePatrollingPoints();

public:
	void Aiming() const;
	void MoveToNextPoint(float DeltaTime);
	
private:
	UPROPERTY()
	AEnemyTank* MyPawn;
	UPROPERTY()
	AUnitBase* MyTarget;
	UPROPERTY()
	TArray<FVector> PatrollingPoints;
	UPROPERTY()
	int32 CurrentPatrolPointIndex = 0;

	float TargetForwardAxisValue, CurrentForwardAxisValue;
	float TargetRotationAxisValue, CurrentRotationAxisValue;
};
