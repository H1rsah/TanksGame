// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerTank.h"

#include "Kismet/KismetMathLibrary.h"

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

	SetupCannon(CannonType);
}

void APlayerTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// Tank movement
	CurrentForwardAxisValue = FMath::Lerp(CurrentForwardAxisValue, TargetForwardAxisValue, MoveInterpolation);
	FVector MoveVector = GetActorForwardVector() * CurrentForwardAxisValue;
	SetActorLocation(GetActorLocation() + MoveVector * MoveSpeed * DeltaTime,true);
	
	// Tank rotation
	CurrentRotationAxisValue = FMath::Lerp(CurrentRotationAxisValue, TargetRotationAxisValue, RotationInterpolation);
	FRotator CurrentRotation = GetActorRotation();
	CurrentRotation.Yaw += RotationSpeed * CurrentRotationAxisValue * DeltaTime;
	SetActorRotation(CurrentRotation);
	
	// Turret rotation
	if(TankController)
	{
		if (TankController->bIsControllingFromMouse)
		{
			RotateTurret();
		}
		else
		{
			CurrentTurretRotationAxisValue = FMath::Lerp(CurrentTurretRotationAxisValue, TurretRotationAxisValue, TurretRotationInterpolation);
			FRotator CurrentTurretRotation = TurretMesh->GetComponentRotation();
			CurrentTurretRotation.Yaw += RotationSpeed * CurrentTurretRotationAxisValue * DeltaTime;
			TurretMesh->SetWorldRotation(CurrentTurretRotation);
		}
	}

	// Score
	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Black, FString::Printf(TEXT("Score: %d"), TankController->PlayerScore));
}

void APlayerTank::TakeDamage(const FDamageTypes& DamageType)
{
	Super::TakeDamage(DamageType);

	if(HitForceEffect)
	{
		FForceFeedbackParameters EffectParams;
		EffectParams.bLooping = false;
		EffectParams.Tag = "EffectParams";
		GetWorld()->GetFirstPlayerController()->ClientPlayForceFeedback(HitForceEffect, EffectParams);
	}
	if(HitShake)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitShake);
	}
}

void APlayerTank::MoveForward(float AxisValue)
{
	TargetForwardAxisValue = AxisValue;
}

void APlayerTank::RotateRight(float AxisValue)
{
	TargetRotationAxisValue = AxisValue;
}

void APlayerTank::TurretRotateRight(float AxisValue)
{
	TurretRotationAxisValue = AxisValue;
}

void APlayerTank::RotateTurret()
{
	const FVector TargetPosition = TankController->GetShootTarget();
	const FRotator CurrentRotation = TurretMesh->GetComponentRotation();
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(TurretMesh->GetComponentLocation(), TargetPosition);
	TargetRotation.Pitch = CurrentRotation.Pitch;
	TargetRotation.Roll = CurrentRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::Lerp(CurrentRotation, TargetRotation, TurretRotationInterpolation));
}

void APlayerTank::FireSpecial()
{
	if(CurrentCannon)
		CurrentCannon->FireSpecial();
}

void APlayerTank::SwitchCannon()
{
	Swap(CurrentCannon, ReserveCannon);
	if (CurrentCannon)
	{
		CurrentCannon->SetVisibility(true);
	}

	if (ReserveCannon)
	{
		ReserveCannon->SetVisibility(false);
	}
}

void APlayerTank::SetTurretTargetPosition()
{
	RotateTurret();
}

FVector APlayerTank::GetTurretForwardVector() const
{
	return TurretMesh->GetForwardVector();
}
