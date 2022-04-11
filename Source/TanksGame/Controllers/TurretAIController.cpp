// Fill out your copyright notice in the Description page of Project Settings.
#pragma optimize( "", off )

#include "TurretAIController.h"
#include "DrawDebugHelpers.h"
#include "EnemyTower.h"
#include "WaypointActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


void ATurretAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	MyPawn = Cast<AEnemyTower>(InPawn);
}

void ATurretAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATurretAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MyPawn)
		GetClosestTarget();

	if (MyTarget)
		Aiming();
}

void ATurretAIController::GetClosestTarget()
{
	TArray<AActor*> PotentialTargets;
	TArray<AActor*> TmpTargets;
	float DistanceToTarget;

	for (const auto EnemyTag : MyPawn->EnemyTags)
	{
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), EnemyTag, TmpTargets);
		PotentialTargets.Append(TmpTargets);
	}

	AActor* TmpActor = UGameplayStatics::FindNearestActor(MyPawn->GetActorLocation(), PotentialTargets, DistanceToTarget);
	if (MyPawn->TargetingRange > DistanceToTarget)
	{

		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.bReturnPhysicalMaterial = false;
		Params.AddIgnoredActor(MyPawn);
		Params.bTraceComplex = true;
		// TArray<AActor*> ActorsToIgnore = {MyPawn};
		bool bWeHitSomething = UKismetSystemLibrary::LineTraceSingle(this,
			MyPawn->GetCannonLocation(), TmpActor->GetActorLocation(),
			UEngineTypes::ConvertToTraceType(ECC_Pawn),
			false,
			{MyPawn},
			EDrawDebugTrace::None,
			Hit,
			true,
			FLinearColor::Green,
			FLinearColor::Red,
			0);
		// if (bWeHitSomething)
		// 	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Black,FString::Printf(TEXT("Hit: %s"), *Hit.Actor->GetName()));
		// else
		// 	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Yellow,"No Fish!");
		
		if (Hit.Actor == TmpActor)
			MyTarget = Cast<AUnitBase>(TmpActor);
		else
			MyTarget = nullptr;
	}
	else
	{
		MyTarget = nullptr;
	}
}

void ATurretAIController::Aiming() const
{
	// DrawDebugLine(GetWorld(), MyPawn->GetActorLocation(), MyTarget->GetActorLocation(), FColor::Yellow, false, -1, 0, 3);
	// GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Yellow,"Aiming!");

	MyPawn->RotateTurretToTarget(MyTarget->GetActorLocation(), true);
    
	if (MyPawn->CanFire(MyTarget->GetActorLocation()))
	{
		MyPawn->Fire();
	}
}

#pragma optimize( "", on )