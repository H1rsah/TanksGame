// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerTankController.generated.h"


class APlayerTank;
/**
 * 
 */
UCLASS()
class TANKSGAME_API APlayerTankController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	APlayerTank* PlayerTankPawn;
	FVector MousePosition;
	
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

public:
	FVector GetShootVector() const { return MousePosition; };

	UPROPERTY()
	int32 PlayerScore;
	bool bIsControllingFromMouse = true;

	

private:
	void MoveForward(float AxisValue);
	void RotateRight(float AxisValue);
	void TurretRotateRight(float AxisValue);
	void Fire();
	void FireSpecial();
	void SwitchCannon();
};