// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/AudioComponent.h"
#include "TanksGame/Interfaaces/Damageable.h"


// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickInterval = 0.05f;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	Mesh->SetHiddenInGame(true);

	AudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Effect"));
	AudioEffect->SetupAttachment(RootComponent);
}


void AProjectile::Start()
{
	PrimaryActorTick.SetTickFunctionEnable(true);
	StartPosition = GetActorLocation();
	Mesh->SetHiddenInGame(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AProjectile::Stop()
{
	PrimaryActorTick.SetTickFunctionEnable(false);
	Mesh->SetHiddenInGame(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	UActorPool* Pool = GetWorld()->GetSubsystem<UActorPool>();
	if (Pool->IsActorInPool(this))
	{
		Pool->ReturnActor(this);
	}
	else
	{
		Destroy();
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NextPosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * DeltaTime;
	SetActorLocation(NextPosition, true);

	if (FVector::Dist(GetActorLocation(), StartPosition) > FireRange)
	{
		Stop();
	}
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& SweepResult)
{
	if (OtherActor == GetInstigator())
	{
		Destroy();
		return;
	}
	
	if (OtherActor && OtherComp && OtherComp->GetCollisionObjectType() == ECC_Destructible)
	{
		OtherActor->Destroy();
	}
	else if (IDamageable* Damageable = Cast<IDamageable>(OtherActor))
	{
		FDamageTypes DamageType;
		DamageType.DamageValue = Damage;
		DamageType.Instigator = GetInstigator();
		DamageType.DamageMaker = this;
		Damageable->TakeDamage(DamageType);
	}

	AudioEffect->Play();
	Stop();
}


