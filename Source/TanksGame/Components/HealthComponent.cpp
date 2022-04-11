// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"
#include "TanksGame/TanksGameGameModeBase.h"


UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UHealthComponent::TakeDamage(FDamageTypes DamageType)
{
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
	
	OnObtainDamage.Broadcast();
	// GEngine->AddOnScreenDebugMessage(-1, 0.2, FColor::Red, FString::Printf(TEXT("Health (%f)"), CurrentHealth));
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}