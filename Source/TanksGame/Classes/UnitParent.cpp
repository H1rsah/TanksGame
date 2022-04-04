// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitParent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AUnitParent::AUnitParent()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit Box"));
	RootComponent = HitBox;
	
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank body"));
	BodyMesh->SetupAttachment(RootComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank turret"));
	TurretMesh->SetupAttachment(BodyMesh);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon"));
	CannonSetupPoint->SetupAttachment(TurretMesh);
}

void AUnitParent::RotateTurretTo(FVector TargetPosition)
{
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(TurretMesh->GetComponentLocation(), TargetPosition);
	const FRotator CurrentRotation = TurretMesh->GetComponentRotation();
	TargetRotation.Pitch = CurrentRotation.Pitch;
	TargetRotation.Roll = CurrentRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::Lerp(CurrentRotation, TargetRotation, TurretRotationInterpolation));

}

void AUnitParent::Fire()
{
	if(CurrentCannon)
		CurrentCannon->Fire();
}

void AUnitParent::FireSpecial()
{
	if(CurrentCannon)
		CurrentCannon->FireSpecial();
}

void AUnitParent::SetupCannon(TSubclassOf<ACannon> NewCannon)
{
	if (CurrentCannon)
	{
		CurrentCannon->Destroy();
	}
	
	if (NewCannon)
	{
		FActorSpawnParameters Params;
		Params.Owner = this;
		CurrentCannon = GetWorld()->SpawnActor<ACannon>(NewCannon, CannonSetupPoint->GetComponentTransform(), Params);
		CurrentCannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
}

// Called when the game starts or when spawned
void AUnitParent::BeginPlay()
{
	Super::BeginPlay();

	SetupCannon(Cannon);
}

// Called every frame
void AUnitParent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}