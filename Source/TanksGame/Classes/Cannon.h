// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "TanksGame/Structures/GameStructures.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Camera/CameraShakeBase.h"
#include "GameFramework/ForceFeedbackEffect.h"
#include "Cannon.generated.h"

UCLASS()
class TANKSGAME_API ACannon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACannon();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent * ProjectileSpawnPoint;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* ShootEffect;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UAudioComponent* AudioEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireRate = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireRange = 3000;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireDamage = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	bool bHasSpecialFire = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float LaserDamage = 5;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	int32 MaxAmmo = 10;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = 1), Category = "Fire params")
	int32 BurstShotsAmount = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "BurstShotsAmount > 1", EditConditionHides), Category = "Fire params")
	float BurstLength = 1.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	ECannonType CannonType = ECannonType::FireProjectile;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "CannonType == ECannonType::FireProjectile", EditConditionHides), Category = "Fire params")
	TSubclassOf<AProjectile> ProjectileActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	UForceFeedbackEffect* ShootForceEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	TSubclassOf<UCameraShakeBase> ShootShake;
private:
	FTimerHandle ReloadTimerHandle;
	FTimerHandle BurstTimerHandle;
	bool bIsReadyToFire = false;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Fire();
	void FireSpecial();
	bool IsReadyToFire() const;
	bool HasSpecialFire() const;
	void AddAmmo(int32 Value);
	void SetVisibility(bool bIsVisible) const;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
	
	void Reload();
	void Shot();

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentAmmo = 0;
	int32 BurstShotsLeft = 0;
};
