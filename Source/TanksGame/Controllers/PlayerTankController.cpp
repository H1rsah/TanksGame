// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerTankController.h"
#include "DrawDebugHelpers.h"
#include "PlayerTank.h"

void APlayerTankController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &APlayerTankController::MoveForward);
	InputComponent->BindAxis("RotateRight", this, &APlayerTankController::RotateRight);
	InputComponent->BindAxis("TurretRotateRight", this, &APlayerTankController::TurretRotateRight);
	InputComponent->BindAction("Fire", IE_Pressed, this, &APlayerTankController::Fire);
	InputComponent->BindAction("FireSpecial", IE_Pressed, this, &APlayerTankController::FireSpecial);
	InputComponent->BindAction("SwitchCannon",IE_Pressed, this, &APlayerTankController::SwitchCannon);

	bShowMouseCursor = true;
}

void APlayerTankController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FVector MouseLocalPosition, MouseDirection;
	DeprojectMousePositionToWorld(MouseLocalPosition, MouseDirection);
	auto Z = FMath::Abs(PlayerTankPawn->GetActorLocation().Z - MouseLocalPosition.Z);
	MousePosition = MouseLocalPosition - MouseDirection * Z /MouseDirection.Z;
}

void APlayerTankController::BeginPlay()
{
	Super::BeginPlay();

	PlayerTankPawn = Cast<APlayerTank>(GetPawn());
}

void APlayerTankController::MoveForward(float AxisValue)
{
	if(PlayerTankPawn)
		PlayerTankPawn->MoveForward(AxisValue);
}

void APlayerTankController::RotateRight(float AxisValue)
{
	if(PlayerTankPawn)
		PlayerTankPawn->RotateRight(AxisValue);
}

void APlayerTankController::TurretRotateRight(float AxisValue)
{
	if(PlayerTankPawn)
		if (FMath::IsNearlyZero(AxisValue))
			bIsControllingFromMouse = true;
		else
			bIsControllingFromMouse = false;
	PlayerTankPawn->TurretRotateRight(AxisValue);
}

void APlayerTankController::Fire()
{
	if(PlayerTankPawn)
		PlayerTankPawn->Fire();
}

void APlayerTankController::FireSpecial()
{
	if(PlayerTankPawn)
		PlayerTankPawn->FireSpecial();
}

void APlayerTankController::SwitchCannon()
{
	if (PlayerTankPawn)
		PlayerTankPawn->SwitchCannon();
}