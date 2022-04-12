// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorPool.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
#include "TanksGame/Interfaaces/Damageable.h"
#include "ProjectileBase.generated.h"

UCLASS()
class TANKSGAME_API AProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	AProjectileBase();
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UAudioComponent* AudioEffect;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	float Damage = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 500.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float FireRange = 3000.f;


protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& SweepResult);

	void Start();
	void Move(float DeltaTime);
	void Stop();

private:
	FVector StartPosition;
};
