// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyTank.h"

AEnemyTank::AEnemyTank()
{
	PrimaryActorTick.bCanEverTick = true;

	Mobility = EUnitMobility::Mobile;
}

void AEnemyTank::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
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


void AEnemyTank::MoveForward(float AxisValue)
{
	TargetForwardAxisValue = AxisValue;
}

void AEnemyTank::RotateRight(float AxisValue)
{
	TargetRotationAxisValue = AxisValue;
}
