// Fill out your copyright notice in the Description page of Project Settings.
#pragma optimize( "", off )

#include "FactoryBase.h"

#include "Kismet/GameplayStatics.h"


AFactoryBase::AFactoryBase()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root Scene Component");
	
	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>("Building Mesh");
	BuildingMesh->SetupAttachment(RootComponent);

	HitBox = CreateDefaultSubobject<UBoxComponent>("Hit Box");
	HitBox->SetupAttachment(RootComponent);

	SpawnPoint = CreateDefaultSubobject<UArrowComponent>("Unit Spawn Point");
	SpawnPoint->SetupAttachment(BuildingMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health Component");
	HealthComponent->OnDie.AddDynamic(this, &AFactoryBase::OnDie);
}

void AFactoryBase::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AFactoryBase::OnTankSpawnTick, SpawnInterval, true);
}

void AFactoryBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFactoryBase::TakeDamage(const FDamageTypes& DamageData)
{
	if(HealthComponent)
		HealthComponent->TakeDamage(DamageData);
}

void AFactoryBase::OnTankSpawnTick()
{
	SpawnUnit();
}

void AFactoryBase::SpawnUnit()
{
	if (SpawnAmount > 0)
	{
		auto Tank = GetWorld()->SpawnActorDeferred<AEnemyTank>(SpawnClass, SpawnPoint->GetComponentTransform());
		Tank->WaypointTag = WaypointTag;
		Tank->EnemyTags = EnemyTags;
		Tank->Tags = TeamTags;
		UGameplayStatics::FinishSpawningActor(Tank, SpawnPoint->GetComponentTransform());

		SpawnAmount--;
	}
	else
	{
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
	}
}

int32 AFactoryBase::GetScores() const
{
	return DestructionScore;
}

void AFactoryBase::OnDie_Implementation()
{
	if (LevelTrigger)
		LevelTrigger->ChangeFlag(true);

	GetWorldTimerManager().ClearTimer(SpawnTimerHandle);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DieParticle, GetActorLocation(), FRotator::ZeroRotator, FVector(10), true, EPSCPoolMethod::None, true);
	Destroy();

}

void AFactoryBase::OnHealthChanged_Implementation(float Damage)
{
}


#pragma optimize( "", on )
