// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Called to bind functionality to input
void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATankPawn::TakeDamage(const FDamageTypes& Damage)
{
	if(HealthComponent)
		HealthComponent->TakeDamage(Damage);
}

void ATankPawn::OnHealthChanged(float X)
{
	GEngine->AddOnScreenDebugMessage(123, 2, FColor::Red, FString::Printf(TEXT("Health: %f; Damage: %f"), HealthComponent->CurrentHealth, X));
}

void ATankPawn::OnDie()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}

// Sets default values
ATankPawn::ATankPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit Box"));
	RootComponent = HitBox;
	
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank body"));
	BodyMesh->SetupAttachment(RootComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank turret"));
	TurretMesh->SetupAttachment(BodyMesh);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon"));
	CannonSetupPoint->SetupAttachment(TurretMesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArm->SetupAttachment(BodyMesh);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;
	SpringArm->TargetArmLength = 2000.f;
	SpringArm->SetRelativeRotation(FRotator(-90.f,0.f,0.f));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health Component");
	HealthComponent->OnHealthChanged.AddDynamic(this, &ATankPawn::OnHealthChanged);
	HealthComponent->OnDie.AddDynamic(this, &ATankPawn::OnDie);
}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();

	TankController = Cast<ATankPlayerController>(GetController());

	SetupCannon(DefaultCannon);
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
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

void ATankPawn::MoveForward(float AxisValue)
{
	TargetForwardAxisValue = AxisValue;
}

void ATankPawn::RotateRight(float AxisValue)
{
	TargetRotationAxisValue = AxisValue;
}

void ATankPawn::RotateTurretTo(FVector TargetPosition) const
{
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(TurretMesh->GetComponentLocation(), TargetPosition);
	const FRotator CurrentRotation = TurretMesh->GetComponentRotation();
	TargetRotation.Pitch = CurrentRotation.Pitch;
	TargetRotation.Roll = CurrentRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::Lerp(CurrentRotation, TargetRotation, TurretRotationInterpolation));
}

void ATankPawn::Fire()
{
	if(CurrentCannon)
		CurrentCannon->Fire();
}

void ATankPawn::FireSpecial()
{
	if(CurrentCannon)
		CurrentCannon->FireSpecial();
}

void ATankPawn::SetupCannon(TSubclassOf<ACannon> NewCannon)
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

void ATankPawn::SwitchCannon()
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

ACannon* ATankPawn::GetActiveCannon() const
{
	return CurrentCannon;
}

void ATankPawn::SetTurretTargetPosition(const FVector& Target)
{
	RotateTurretTo(Target);
}

FVector ATankPawn::GetTurretForwardVector()
{
	return TurretMesh->GetForwardVector();
}

