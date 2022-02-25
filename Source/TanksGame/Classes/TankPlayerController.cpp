#include "TankPlayerController.h"

#include "DrawDebugHelpers.h"
#include "TankPawn.h"

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &ATankPlayerController::MoveForward);
	InputComponent->BindAxis("RotateRight", this, &ATankPlayerController::RotateRight);
	InputComponent->BindAction("Fire", IE_Pressed, this, &ATankPlayerController::Fire);
	InputComponent->BindAction("FireSpecial", IE_Pressed, this, &ATankPlayerController::FireSpecial);


	bShowMouseCursor = true;
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector MouseLocalPosition, MouseDirection;
	DeprojectMousePositionToWorld(MouseLocalPosition, MouseDirection);
	auto Z = FMath::Abs(TankPawn->GetActorLocation().Z - MouseLocalPosition.Z);
	MousePosition = MouseLocalPosition - MouseDirection * Z /MouseDirection.Z;
	// DrawDebugLine(GetWorld(), MouseLocalPosition, MouseLocalPosition + MouseDirection * 5000, FColor::Green, false, 0.1f, 0, 5);
}

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TankPawn = Cast<ATankPawn>(GetPawn());
}

void ATankPlayerController::MoveForward(float AxisValue)
{
	if(TankPawn)
		TankPawn->MoveForward(AxisValue);
}

void ATankPlayerController::RotateRight(float AxisValue)
{
	if(TankPawn)
		TankPawn->RotateRight(AxisValue);
}

void ATankPlayerController::Fire()
{
	if(TankPawn)
		TankPawn->Fire();
}

void ATankPlayerController::FireSpecial()
{
	if(TankPawn)
		TankPawn->FireSpecial();
}
