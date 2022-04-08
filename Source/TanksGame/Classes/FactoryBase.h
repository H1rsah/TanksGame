// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyTank.h"
#include "LevelTrigger.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "TanksGame/Components/HealthComponent.h"
#include "TanksGame/Interfaaces/Damageable.h"
#include "FactoryBase.generated.h"

UCLASS()
class TANKSGAME_API AFactoryBase : public AActor, public IDamageable, public IScoreable
{
	GENERATED_BODY()

	AFactoryBase();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void TakeDamage(const FDamageTypes& DamageData) override;

	FTimerHandle SpawnTimerHandle;
	void OnTankSpawnTick();
	void SpawnUnit();
	

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Health")
	void OnHealthChanged(float Damage);
	UFUNCTION(BlueprintNativeEvent, Category = "Health")
	void OnDie();
	virtual int32 GetScores() const override;


public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BuildingMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* HitBox;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* SpawnPoint;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMesh* RuinsMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* DieParticle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* RuinsParticle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory")
	TSubclassOf<AEnemyTank> SpawnClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory")
	int32 SpawnAmount = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory")
	float SpawnInterval = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory")
	FName WaypointTag = "Default";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory")
	int DestructionScore = 20;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory|AI Team Settingss")
	TArray<FName> EnemyTags;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Factory|AI Team Settingss")
	TArray<FName> TeamTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	ALevelTrigger* LevelTrigger;
	
};
