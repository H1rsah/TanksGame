// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TanksGame/Structures/S_DamageTypes.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDie);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, DamageValue);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TANKSGAME_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();


	void TakeDamage(FDamageTypes DamageType);
	float GetHealth() const { return CurrentHealth; };

	
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FOnDie OnDie;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxHealth = 10.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CurrentHealth;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
