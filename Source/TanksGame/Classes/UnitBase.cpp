// Fill out your copyright notice in the Description page of Project Settings.

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
	
	WidgetHealthBar = CreateDefaultSubobject<UWidgetComponent>("Health Bar");
	WidgetHealthBar->SetupAttachment(RootComponent);
	
	WidgetObtainedDamage = CreateDefaultSubobject<UWidgetComponent>("Obtained Damage");
	WidgetObtainedDamage->SetupAttachment(RootComponent);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	HealthComponent->OnHealthChanged.AddDynamic(this, &AUnitBase::OnHealthChanged);
	HealthComponent->OnDie.AddDynamic(this, &AUnitBase::OnDie);
}

void AUnitBase::BeginPlay()
{
	Super::BeginPlay();

	ObtainedDamage = Cast<UObtainedDamageWidget>(WidgetObtainedDamage->GetUserWidgetObject());
	
	if (UHealthBarWidget* Health = Cast<UHealthBarWidget>(WidgetHealthBar->GetUserWidgetObject()))
	{
		Health->SetOwnerUnit(HealthComponent);
		WidgetHealthBar->SetVisibility(bShowHealthWidgetInGame);
	}
	
	if (CannonClass)
	{
		FActorSpawnParameters Params;
		Params.Owner = this;
		CurrentCannon = GetWorld()->SpawnActor<ACannon>(CannonClass, Params);
		CurrentCannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		CurrentCannon->MaxAmmo = 10;
		
	}
}

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
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(CannonSetupPoint->GetComponentLocation(), TargetLocation);
	if (bIsRollRequired)
		TargetRotation.Roll = 90.f;

	const FRotator TurretRotation = TurretMesh->GetComponentRotation();

	TurretMesh->SetWorldRotation(FMath::Lerp(TurretRotation, TargetRotation, TargetingSpeed));
	
	// GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Black, FString::Printf(TEXT("TargetLocation: (%f; %f; %f)    TargetRotation: (%f; %f; %f)   TurretRotation(): (%f; %f; %f)"), TargetLocation.X, TargetLocation.Y, TargetLocation.Z, TargetRotation.Roll, TargetRotation.Pitch, TargetRotation.Yaw, TurretRotation.Roll, TurretRotation.Pitch, TurretRotation.Yaw));
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

ACannon* AUnitBase::GetActiveCannon() const
{
	return CurrentCannon;
}

void AUnitBase::SetupCannon(TSubclassOf<ACannon> NewCannon)
{
	if (CurrentCannon)
	{
		CurrentCannon->Destroy();
	}

	if (NewCannon)
	{
		FActorSpawnParameters Params;
		Params.Instigator = this;
		Params.Owner = this;
		CurrentCannon = GetWorld()->SpawnActor<ACannon>(NewCannon, CannonSetupPoint->GetComponentTransform(), Params);
		CurrentCannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
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
	// GEngine->AddOnScreenDebugMessage(123, 2, FColor::Red, FString::Printf(TEXT("TURRET Health: %f; Damage: %f"), HealthComponent->CurrentHealth, Damage));
}

void AUnitBase::TakeDamage(const FDamageTypes& DamageData)
{
	HealthComponent->TakeDamage(DamageData);
	
	if (ObtainedDamage)
	{
		ObtainedDamage->DamageAmount = DamageData.DamageValue;
		ObtainedDamage->Refresh();
	}
}

int32 AUnitBase::GetScores() const
{
	return DestructionScore;
}