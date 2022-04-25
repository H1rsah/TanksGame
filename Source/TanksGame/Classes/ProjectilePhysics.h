// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "TanksGame/Components/PhysicsMovementComponent.h"
#include "ProjectilePhysics.generated.h"

UCLASS()
class TANKSGAME_API AProjectilePhysics : public AProjectileBase
{
	GENERATED_BODY()

public:
	AProjectilePhysics();

	virtual void Start() override;
	virtual void Stop() override;
	virtual void Move(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UPhysicsMovementComponent* MovementComponent;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
