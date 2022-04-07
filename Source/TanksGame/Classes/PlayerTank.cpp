// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerTank.h"

APlayerTank::APlayerTank()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArm->SetupAttachment(BodyMesh);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;
	SpringArm->TargetArmLength = 3000.f;
	SpringArm->SetRelativeRotation(FRotator(-90.f,0.f,0.f));
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void APlayerTank::BeginPlay()
{
	Super::BeginPlay();

	TankController = Cast<APlayerTankController>(GetController());

	SetupCannon(DefaultCannon);
}

void APlayerTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerTank::SetupCannon(const TSubclassOf<ACannon> NewCannon)
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

void APlayerTank::MoveForward(float AxisValue)
{
}

void APlayerTank::RotateRight(float AxisValue)
{
}

void APlayerTank::RotateTurretTo(FVector TargetPosition) const
{
}
//
// void APlayerTank::Fire()
// {
// }

void APlayerTank::FireSpecial()
{
}

void APlayerTank::SwitchCannon()
{
}

ACannon* APlayerTank::GetActiveCannon() const
{
	return CurrentCannon;
}

void APlayerTank::SetTurretTargetPosition(const FVector& Target)
{
}

FVector APlayerTank::GetTurretForwardVector() const
{
	return TurretMesh->GetForwardVector();
}
