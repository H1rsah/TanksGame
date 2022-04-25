// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsMovementComponent.h"


UPhysicsMovementComponent::UPhysicsMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}


void UPhysicsMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UPhysicsMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	const FVector NewActorLocation = Owner->GetActorLocation() + Velocity * DeltaTime + FVector::UpVector * GetGravity() * FMath::Square(DeltaTime) / 2.f;
	Velocity += FVector::UpVector * GetGravity() * DeltaTime;
	Owner->SetActorLocation(NewActorLocation, true);
}

