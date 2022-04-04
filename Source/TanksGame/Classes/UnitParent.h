// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cannon.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "TanksGame/Components/HealthComponent.h"
#include "UnitParent.generated.h"

UCLASS()
class TANKSGAME_API AUnitParent : public AActor
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	AUnitParent();

	UFUNCTION(BlueprintCallable, Category = "Turret")
	void RotateTurretTo(FVector TargetPosition);
	UFUNCTION(BlueprintCallable, Category = "Turret")
	void Fire();
	UFUNCTION(BlueprintCallable, Category = "Turret")
	void FireSpecial();
	UFUNCTION(BlueprintCallable, Category = "Turret")
	void SetupCannon(TSubclassOf<ACannon> NewCannon);

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
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Cannon")
	TSubclassOf<ACannon> Cannon;

	float RotationSpeed = 100;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float TurretRotationInterpolation = 0.05f;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	ACannon* CurrentCannon = nullptr;
};
