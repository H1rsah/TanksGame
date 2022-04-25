// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectilePhysics.h"
#include "TanksGame/Components/PhysicsMovementComponent.h"


AProjectilePhysics::AProjectilePhysics()
{
	MovementComponent = CreateDefaultSubobject<UPhysicsMovementComponent>(TEXT("Movement Component"));
}

void AProjectilePhysics::Start()
{
	Super::Start();

	MovementComponent->Velocity = GetActorForwardVector() * MoveSpeed;
	MovementComponent->SetComponentTickEnabled(true);
}

void AProjectilePhysics::Stop()
{
	MovementComponent->Velocity = FVector::ZeroVector;
	MovementComponent->SetComponentTickEnabled(false);
	
	Super::Stop();
}

void AProjectilePhysics::Move(float DeltaTime)
{
	Super::Move(DeltaTime);
}

void AProjectilePhysics::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectilePhysics::Tick(float DeltaTime)
{
	if (GetActorLocation().Z < -10000.f)
	{
		Stop();
	}
	// Super::Tick(DeltaTime);
}

