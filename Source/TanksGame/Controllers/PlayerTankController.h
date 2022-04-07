// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TanksGame/Interfaaces/Targetable.h"
#include "PlayerTankController.generated.h"


class APlayerTank;
/**
 * 
 */
UCLASS()
class TANKSGAME_API APlayerTankController : public APlayerController, public ITargetable
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	APlayerTank* PlayerTankPawn;
	FVector MousePosition;

public:
	// Called to bind functionality to input
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaTime) override;
	virtual FVector GetShootTarget() const override { return MousePosition; };

	UPROPERTY()
	int32 PlayerScore;

private:
	virtual void BeginPlay() override;
	void MoveForward(float AxisValue);
	void RotateRight(float AxisValue);
	void Fire();
	void FireSpecial();
	void SwitchCannon();
};