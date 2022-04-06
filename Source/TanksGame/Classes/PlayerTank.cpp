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

	TankController = Cast<ATankPlayerController>(GetController());
}

void APlayerTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// void APlayerTank::BeginPlay()
// {
	// Super::BeginPlay();
	//
	// TankController = Cast<ATankPlayerController>(GetController());
// }

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
		RotateTurretTo(TankController->GetShootTarget());
	}

	// Score
	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Black, FString::Printf(TEXT("Score: %d"), TankController->PlayerScore));
}

void APlayerTank::MoveForward(float AxisValue)
{
	TargetForwardAxisValue = AxisValue;
}

void APlayerTank::RotateRight(float AxisValue)
{
	TargetRotationAxisValue = AxisValue;
}

void APlayerTank::RotateTurretTo(FVector TargetPosition) const
{
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(TurretMesh->GetComponentLocation(), TargetPosition);
	const FRotator CurrentRotation = TurretMesh->GetComponentRotation();
	TargetRotation.Pitch = CurrentRotation.Pitch;
	TargetRotation.Roll = CurrentRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::Lerp(CurrentRotation, TargetRotation, TurretRotationInterpolation));
}

void APlayerTank::Fire()
{
	if(CurrentCannon)
		CurrentCannon->Fire();
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

void APlayerTank::SetTurretTargetPosition(const FVector& Target)
{
	RotateTurretTo(Target);
}

FVector APlayerTank::GetTurretForwardVector() const
{
	return TurretMesh->GetForwardVector();
}

void APlayerTank::TakeDamage(const FDamageTypes& Damage)
{
	Super::TakeDamage(Damage);
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

ACannon* APlayerTank::GetActiveCannon() const
{
	return CurrentCannon; 
}
