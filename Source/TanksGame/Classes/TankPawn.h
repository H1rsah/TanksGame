// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Cannon.h"
#include "TankPlayerController.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Pawn.h"
#include "TankPawn.generated.h"

UCLASS()
class TANKSGAME_API ATankPawn : public APawn
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UBoxComponent* HitBox;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* BodyMesh;
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* TurretMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UCameraComponent* Camera;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent * CannonSetupPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
	TSubclassOf<ACannon> DefaultCannon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Cannon")
	TSubclassOf<ACannon> CannonType;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
	ATankPlayerController* TankController;

public:
	// Sets default values for this pawn's properties
	ATankPawn();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveForward(float AxisValue);
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void RotateRight(float AxisValue);
	UFUNCTION(BlueprintCallable, Category = "Turret")
	void RotateTurretTo(FVector TargetPosition) const;
	UFUNCTION(BlueprintCallable, Category = "Turret")
	void Fire();
	UFUNCTION(BlueprintCallable, Category = "Turret")
	void FireSpecial();
	UFUNCTION(BlueprintCallable, Category = "Turret")
	void SetupCannon(TSubclassOf<ACannon> NewCannon);
	UFUNCTION(BlueprintCallable, Category = "Turret")
	void SwitchCannon();
	UFUNCTION(BlueprintPure, Category = "Turret")
	ACannon* GetActiveCannon() const;

	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	float TargetForwardAxisValue, CurrentForwardAxisValue;
	float TargetRotationAxisValue, CurrentRotationAxisValue;

	UPROPERTY()
	ACannon* CurrentCannon = nullptr;
	UPROPERTY()
	ACannon* ReserveCannon = nullptr;
};
