// Fill out your copyright notice in the Description page of Project Settings.
#pragma optimize( "", off )


#include "UnitBase.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AUnitBase::AUnitBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit Box"));
	RootComponent = HitBox;
	
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("Body");
	BodyMesh->SetupAttachment(RootComponent);
	
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>("Turret");
	TurretMesh->SetupAttachment(BodyMesh);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>("Cannon Setup Point");
	CannonSetupPoint->SetupAttachment(TurretMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	
	HealthComponent->OnHealthChanged.AddDynamic(this, &AUnitBase::OnHealthChanged);
	HealthComponent->OnDie.AddDynamic(this, &AUnitBase::OnDie);
}

void AUnitBase::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters Params;
	Params.Owner = this;
	CurrentCannon = GetWorld()->SpawnActor<ACannon>(CannonClass, Params);
	CurrentCannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

// Called to bind functionality to input
void AUnitBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AUnitBase::Destroyed()
{
	Super::Destroyed();
	
	if (CurrentCannon)
		CurrentCannon->Destroy();
}

void AUnitBase::RotateTurretToTarget(const FVector TargetLocation, const bool bIsRollRequired) const
{
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(TurretMesh->GetComponentLocation(), TargetLocation);
	if (bIsRollRequired)
		TargetRotation.Roll = 90.f;

	const FRotator TurretRotation = TurretMesh->GetComponentRotation();
	// GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Black, FString::Printf(TEXT("TargetRotation: (%f; %f; %f)   TurretRotation(): (%f; %f; %f)"), TargetRotation.Roll, TargetRotation.Pitch, TargetRotation.Yaw, TurretRotation.Roll, TurretRotation.Pitch, TurretRotation.Yaw));

	TurretMesh->SetWorldRotation(FMath::Lerp(TurretRotation, TargetRotation, TargetingSpeed));
}

bool AUnitBase::CanFire(const FVector TargetLocation) const
{
	const FVector TargetingDir = TurretMesh->GetForwardVector();
	FVector DirToPlayer = TargetLocation - TurretMesh->GetComponentLocation();
	DirToPlayer.Normalize();

	return Accuracy >= FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(TargetingDir, DirToPlayer)));
}

void AUnitBase::Fire() const
{
	if (CurrentCannon)
	{
		CurrentCannon->Fire();
	}
}

void AUnitBase::SetAmmo(const int32 Amount) const
{
	if (CurrentCannon)
	{
		CurrentCannon->MaxAmmo = Amount;
		CurrentCannon->AddAmmo(Amount);
	}
}

void AUnitBase::OnDie_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DieParticle, GetActorLocation(), FRotator::ZeroRotator, FVector(3), true, EPSCPoolMethod::None, true);
	Destroy();
}

void AUnitBase::OnHealthChanged_Implementation(float Damage)
{
	GEngine->AddOnScreenDebugMessage(123, 2, FColor::Red, FString::Printf(TEXT("TURRET Health: %f; Damage: %f"), HealthComponent->CurrentHealth, Damage));
}

void AUnitBase::TakeDamage(const FDamageTypes& DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

int32 AUnitBase::GetScores() const
{
	return DestructionScore;
}

#pragma optimize( "", on )