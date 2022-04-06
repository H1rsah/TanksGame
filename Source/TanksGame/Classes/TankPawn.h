// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Cannon.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Pawn.h"
#include "TanksGame/Components/HealthComponent.h"
#include "TanksGame/Controllers/TankPlayerController.h"
#include "TanksGame/Interfaaces/Damageable.h"
#include "TanksGame/Interfaaces/Scoreable.h"
#include "TankPawn.generated.h"

UCLASS()
class TANKSGAME_API ATankPawn : public APawn, public IDamageable, public IScoreable
{
	GENERATED_BODY()
	// Sets default values for this pawn's properties
	ATankPawn();

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* HitBox;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BodyMesh;
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* TurretMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UCameraComponent* Camera;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent * CannonSetupPoint;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params")
	float MovementAccuracy = 50.f;
	
	UPROPERTY()
	ATankPlayerController* TankController;
	
public:
	UFUNCTION(BlueprintPure, Category = "AI|Move params")
	float GetMovementAccuracy() const { return MovementAccuracy; }
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
	UFUNCTION(BlueprintCallable, Category = "Turret")
	void SetTurretTargetPosition(const FVector& Target);
	UFUNCTION(BlueprintPure, Category = "Turret")
	FVector GetTurretForwardVector();
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move")
	FName WaypointTag;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void TakeDamage(const FDamageTypes& Damage) override;
	void AddScore() { TargetsDestroyed += DestructionScore; };

private:
	float TargetForwardAxisValue, CurrentForwardAxisValue;
	float TargetRotationAxisValue, CurrentRotationAxisValue;
	int32 TargetsDestroyed = 0, DestructionScore = 1;

	UPROPERTY()
	ACannon* CurrentCannon = nullptr;
	UPROPERTY()
	ACannon* ReserveCannon = nullptr;
	
	UFUNCTION()
	void OnHealthChanged(float X);
	UFUNCTION()
	void OnDie();
};
