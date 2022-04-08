// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitBase.h"
#include "EnemyTank.generated.h"

/**
 * 
 */
UCLASS()
class TANKSGAME_API AEnemyTank : public AUnitBase
{
	GENERATED_BODY()

	AEnemyTank();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaSeconds) override;

	float TargetForwardAxisValue, CurrentForwardAxisValue;
	float TargetRotationAxisValue, CurrentRotationAxisValue;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Speed")
	float MoveSpeed = 500;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float RotationSpeed = 100;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float MoveInterpolation = 0.1f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float RotationInterpolation = 0.1f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float TurretRotationInterpolation = 0.05f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|AI move params")
	float MovementAccuracy = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|AI move params", meta = (ClampMin = 5.f, ClampMax = 175.f))
	float RotationOnPlaceAngle = 30.f;
	
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveForward(float AxisValue);
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void RotateRight(float AxisValue);
	
	UFUNCTION()
	float GetMovementAccuracy() const { return MovementAccuracy; }
};
