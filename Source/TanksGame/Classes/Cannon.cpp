// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"


// Sets default values
ACannon::ACannon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Cannon");
	Mesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>("Spawn point");
	ProjectileSpawnPoint->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
	
	BurstShotsLeft = 0;
	AmmoAmount = MaxAmmo;
	bIsReadyToFire = true;
}

// Called every frame
void ACannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Cyan,FString::Printf(TEXT("Ammo: %d"), AmmoAmount));
}

void ACannon::Fire() 
{
	if(!IsReadyToFire())
		return;
	
	bIsReadyToFire = false;
	--AmmoAmount;
	BurstShotsLeft = 1;
	Shot();
}

void ACannon::FireSpecial()
{
	if (!HasSpecialFire() || !IsReadyToFire())
		return;

	bIsReadyToFire = false;
	--AmmoAmount;
	BurstShotsLeft = BurstShotsAmount;

	if (CannonType == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, TEXT("Fire special - projectile"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, TEXT("Fire special - trace"));
	}
	Shot();
}

bool ACannon::IsReadyToFire()
{
    return bIsReadyToFire && AmmoAmount > 0 && BurstShotsLeft == 0;
}

bool ACannon::HasSpecialFire() const
{
	return bHasSpecialFire;
}

void ACannon::Reload()
{
	bIsReadyToFire = true;
}

void ACannon::Shot()
{
	
	check(BurstShotsLeft > 0);
	switch (CannonType)
	{
	case ECannonType::FireProjectile:
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green,"Fire Projectile");
		break;
	case ECannonType::FireTrace:
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red,"Fire Trace");
		break;
	}

	if (--BurstShotsLeft > 0)
	{
		const float NextShotTime = BurstLength / (BurstShotsAmount - 1);
		GetWorld()->GetTimerManager().SetTimer(BurstTimerHandle, this, &ACannon::Shot, NextShotTime, false);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1.f / FireRate, false);
	}
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1 / FireRate, false);
}
