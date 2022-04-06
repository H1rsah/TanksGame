// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyTower.h"
#include "UnitBase.h"
#include "TurretAIController.generated.h"

/**
 * 
 */
UCLASS()
class TANKSGAME_API ATurretAIController : public AAIController
{
	GENERATED_BODY()

	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	void GetClosestTarget();

public:
	void Aiming() const;

private:
	UPROPERTY()
    AEnemyTower* MyPawn;
	UPROPERTY()
	AUnitBase* MyTarget;
};
