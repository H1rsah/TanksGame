// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"
#include "TanksGame/TanksGameGameModeBase.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UHealthComponent::TakeDamage(FDamageTypes DamageType)
{
	// float PrevValue = CurrentHealth;
	CurrentHealth -= DamageType.DamageValue;

	if(CurrentHealth <= 0.f)
	{
		if(OnDie.IsBound())
			OnDie.Broadcast();
		
		Cast<ATanksGameGameModeBase>(GetWorld()->GetAuthGameMode())->NotifyActorWasDestroyedByDamage(GetOwner(), DamageType);
	}
	else
		if(OnHealthChanged.IsBound())
			OnHealthChanged.Broadcast(DamageType.DamageValue);
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}