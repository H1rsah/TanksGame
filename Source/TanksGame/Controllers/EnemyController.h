// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankPawn.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class TANKSGAME_API AEnemyController : public AAIController
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	ATankPawn* TankPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move", Meta = (MakeEditWidget = true))
	TArray<FVector> PatrollingPoints;
	
  	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Move")
  	float Accuracy = 10.f;
  
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetingRange = 3000.f;
 
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void MoveToNextPoint();
	void Targeting();

private:
	int32 CurrentPatrolPointIndex = 0;
	virtual void OnPossess(APawn* InPawn) override;

public:
	void AddTarget(AActor* Target);
	void RemoveTarget(AActor* Target);

};
