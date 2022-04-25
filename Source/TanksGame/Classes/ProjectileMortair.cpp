// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileMortair.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

AProjectileMortair::AProjectileMortair()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Movement");
	
	TrailEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail effect"));
	TrailEffect->SetupAttachment(RootComponent);
}

void AProjectileMortair::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& SweepResult)
{
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, SweepResult);
	
	UPrimitiveComponent* PrimitiveMesh = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
	if (PrimitiveMesh && PrimitiveMesh->IsSimulatingPhysics())
	{
		FVector ForceVector = OtherActor->GetActorLocation() - GetActorLocation();
		ForceVector.Normalize();
		// PrimitiveMesh->AddImpulse(ForceVector * PushForce, NAME_None, true);
	}
}

void AProjectileMortair::Start()
{
	Super::Start();
}

void AProjectileMortair::Move(float DeltaTime)
{
	FVector CurrentMoveVector = CurrentTrajectory[TragectoryPointIndex] - GetActorLocation();
	CurrentMoveVector.Normalize();
	
	FVector NewLocation = GetActorLocation() + CurrentMoveVector * MoveSpeed * DeltaTime;
	SetActorLocation(NewLocation, true);

	if (FVector::Distance(NewLocation, CurrentTrajectory[TragectoryPointIndex]) <= MoveAccurency)
	{
		TragectoryPointIndex++;
		if (TragectoryPointIndex >= CurrentTrajectory.Num())
			Destroy();
		else
		{
			FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CurrentTrajectory[TragectoryPointIndex]);
			SetActorRotation(NewRotation);
		}
	}
}



