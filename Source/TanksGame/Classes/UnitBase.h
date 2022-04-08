// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Cannon.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "TanksGame/Components/HealthComponent.h"
#include "TanksGame/Interfaaces/Damageable.h"
#include "TanksGame/Interfaaces/Scoreable.h"
#include "UnitBase.generated.h"

UCLASS()
class TANKSGAME_API AUnitBase : public APawn, public IDamageable, public IScoreable
{
	GENERATED_BODY()

public:
	AUnitBase();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	virtual void TakeDamage(const FDamageTypes& DamageData) override;
	virtual int32 GetScores() const override;
	
	void RotateTurretToTarget(FVector TargetLocation, bool bIsRollRequired) const;
	bool CanFire(FVector TargetLocation) const;
	void Fire() const;

	UFUNCTION(BlueprintPure, Category = "Turret")
	ACannon* GetActiveCannon() const;
	UFUNCTION(BlueprintCallable, Category = "Turret")
	void SetupCannon(TSubclassOf<ACannon> NewCannon);
	void SetAmmo(int32 Amount) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float TargetingRange = 2000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	TArray<FName> EnemyTags;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	EUnitMobility Mobility = EUnitMobility::Static;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting", meta = (EditCondition = "Mobility == EUnitMobility::Mobile", EditConditionHides))
	FName WaypointTag = "Default";
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* DieParticle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	UForceFeedbackEffect* HitForceEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	TSubclassOf<UCameraShakeBase> HitShake;

	FVector GetCannonLocation() const { return CannonSetupPoint->GetComponentLocation(); }
	FVector GetTurretLocation() const { return TurretMesh->GetComponentLocation(); }

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* HitBox;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BodyMesh;
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* TurretMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UArrowComponent* CannonSetupPoint;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float Accuracy = 5.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetingSpeed = 0.1f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon")
    TSubclassOf<ACannon> CannonClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scores")
    int32 DestructionScore = 1;

	virtual void BeginPlay() override;
	
	virtual void Destroyed() override;

	UFUNCTION(BlueprintNativeEvent, Category = "Health")
	void OnHealthChanged(float Damage);
	UFUNCTION(BlueprintNativeEvent, Category = "Health")
	void OnDie();

	UPROPERTY()
	ACannon* CurrentCannon = nullptr;
};
