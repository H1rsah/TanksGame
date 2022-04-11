// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "DrawDebugHelpers.h"
#include "TanksGame/Interfaaces/Damageable.h"


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

	ShootEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Shoot Effect"));
	ShootEffect->SetupAttachment(Mesh);
	ShootEffect->bAutoActivate = false;

	AudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Effect"));
	AudioEffect->SetupAttachment(Mesh);
	AudioEffect->bAutoActivate = false;
}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
	
	BurstShotsLeft = 0;
	CurrentAmmo = MaxAmmo;
	bIsReadyToFire = true;
}

void ACannon::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(BurstTimerHandle);
}

// Called every frame
void ACannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACannon::Fire() 
{
	if(!IsReadyToFire())
		return;
	
	bIsReadyToFire = false;
	--CurrentAmmo;
	BurstShotsLeft = 1;
	Shot();
}

void ACannon::FireSpecial()
{
	if (!HasSpecialFire() || !IsReadyToFire())
		return;

	bIsReadyToFire = false;
	--CurrentAmmo;
	BurstShotsLeft = BurstShotsAmount;

	Shot();
}

bool ACannon::IsReadyToFire() const
{
    return bIsReadyToFire && CurrentAmmo > 0 && BurstShotsLeft == 0;
}

bool ACannon::HasSpecialFire() const
{
	return bHasSpecialFire;
}

void ACannon::AddAmmo(const int32 Value)
{
	CurrentAmmo = FMath::Clamp(CurrentAmmo + Value, 0, MaxAmmo);
}

void ACannon::SetVisibility(bool bIsVisible) const
{
	Mesh->SetHiddenInGame(!bIsVisible);
}

void ACannon::Reload()
{
	bIsReadyToFire = true;
}

void ACannon::Shot()
{
	
	check(BurstShotsLeft > 0);
	if (CannonType == ECannonType::FireProjectile)
	{
		// GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green,"Fire Projectile");

		UActorPool* Pool = GetWorld()->GetSubsystem<UActorPool>();
		const FTransform SpawnTransform(ProjectileSpawnPoint->GetComponentRotation(), ProjectileSpawnPoint->GetComponentLocation(), FVector::OneVector);
		AProjectile* MyProjectile = Cast<AProjectile>(Pool->RetreiveActor(ProjectileActor, SpawnTransform));
		if(MyProjectile)
		{
			MyProjectile->SetInstigator(GetInstigator());
		}
		MyProjectile->Start();

		ShootEffect->ActivateSystem();
		if (GetOwner() == GetWorld()->GetFirstPlayerController()->GetPawn())
		{
			if(ShootForceEffect)
			{
				FForceFeedbackParameters EffectParams;
				EffectParams.bLooping = false;
				EffectParams.Tag = "EffectParams";
				GetWorld()->GetFirstPlayerController()->ClientPlayForceFeedback(ShootForceEffect, EffectParams);
			}
			if(ShootShake)
			{
				GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(ShootShake);
			}
		}
	}
	else
	{
		// GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red,"Fire Trace");

        FHitResult Hit;
        FVector TraceStart = ProjectileSpawnPoint->GetComponentLocation();
        FVector TraceEnd = ProjectileSpawnPoint->GetComponentLocation() + ProjectileSpawnPoint->GetForwardVector() * FireRange;
		FCollisionQueryParams TraceParams;
		TraceParams.bReturnPhysicalMaterial = false;
		TraceParams.AddIgnoredActor(GetOwner());
		TraceParams.bTraceComplex = true;
		if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Pawn, TraceParams))
		{
			DrawDebugLine(GetWorld(), TraceStart, Hit.Location, FColor::Red, false, 0.5f, 0, 5.f);
			if (Hit.Actor.IsValid() && Hit.Component.IsValid(), Hit.Component->GetCollisionObjectType() == ECC_Destructible)
			{
				Hit.Actor->Destroy();

			}

			if (IDamageable* Damageable = Cast<IDamageable>(Hit.Actor))
			{
				FDamageTypes DamageType;
				DamageType.DamageMaker = this;
				DamageType.DamageValue = LaserDamage;
				DamageType.Instigator = GetInstigator();
				Damageable->TakeDamage(DamageType);
			}
		}
		else
		{
			DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 0.2f, 0, 5.f);
		}
	}
	AudioEffect->Play();

	if (--BurstShotsLeft > 0)
	{
		const float NextShotTime = BurstLength / (BurstShotsAmount - 1);
		GetWorld()->GetTimerManager().SetTimer(BurstTimerHandle, this, &ACannon::Shot, NextShotTime, false);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1.f / FireRate, false);
	}
}
