// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "TanksGame/Controllers/PlayerTankController.h"
#include "PlayerTank.generated.h"

/**
 * 
 */
UCLASS()
class TANKSGAME_API APlayerTank : public AUnitBase
{
	GENERATED_BODY()

	APlayerTank();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
	TSubclassOf<ACannon> DefaultCannon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Cannon")
	TSubclassOf<ACannon> CannonType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float MoveSpeed = 500;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float RotationSpeed = 100;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float MoveInterpolation = 0.1f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float RotationInterpolation = 0.1f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float TurretRotationInterpolation = 0.05f;
	
	UPROPERTY()
	APlayerTankController* TankController;

public:
	UFUNCTION(BlueprintCallable, Category = "Turret")
	void SetupCannon(TSubclassOf<ACannon> NewCannon);
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveForward(float AxisValue);
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void RotateRight(float AxisValue);
	UFUNCTION(BlueprintCallable, Category = "Turret")
	void RotateTurretTo(FVector TargetPosition) const;
	// UFUNCTION(BlueprintCallable, Category = "Turret")
	// void Fire();
	UFUNCTION(BlueprintCallable, Category = "Turret")
	void FireSpecial();
	UFUNCTION(BlueprintCallable, Category = "Turret")
	void SwitchCannon();
	UFUNCTION(BlueprintPure, Category = "Turret")
	ACannon* GetActiveCannon() const;
	UFUNCTION(BlueprintCallable, Category = "Turret")
	void SetTurretTargetPosition(const FVector& Target);
	UFUNCTION(BlueprintPure, Category = "Turret")
	FVector GetTurretForwardVector() const;

private:
	float TargetForwardAxisValue, CurrentForwardAxisValue;
	float TargetRotationAxisValue, CurrentRotationAxisValue;
	int32 DestructionScore = 1;

	UPROPERTY()
	ACannon* ReserveCannon = nullptr;
};