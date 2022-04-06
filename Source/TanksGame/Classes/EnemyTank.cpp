// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyTank.h"

AEnemyTank::AEnemyTank()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.005f;

	Mobility = EUnitMobility::Mobile;
}

void AEnemyTank::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	// Tank movement
	CurrentForwardAxisValue = FMath::Lerp(CurrentForwardAxisValue, TargetForwardAxisValue, MoveInterpolation);
	FVector MoveVector = GetActorForwardVector() * CurrentForwardAxisValue;
	SetActorLocation(GetActorLocation() + MoveVector * MoveSpeed * DeltaSeconds,true);
	

	// Tank rotation
	CurrentRotationAxisValue = FMath::Lerp(CurrentRotationAxisValue, TargetRotationAxisValue, RotationInterpolation);
	FRotator CurrentRotation = GetActorRotation();
	CurrentRotation.Yaw += RotationSpeed * CurrentRotationAxisValue * DeltaSeconds;
	SetActorRotation(CurrentRotation);
}

void AEnemyTank::SetupCannon(TSubclassOf<ACannon> NewCannon)
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

ACannon* AEnemyTank::GetActiveCannon() const
{
	return CurrentCannon;
}


void AEnemyTank::MoveForward(float AxisValue)
{
	TargetForwardAxisValue = AxisValue;
}

void AEnemyTank::RotateRight(float AxisValue)
{
	TargetRotationAxisValue = AxisValue;
}
