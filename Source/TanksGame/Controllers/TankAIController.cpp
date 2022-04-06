// Fill out your copyright notice in the Description page of Project Settings.
#pragma optimize( "", off )


#include "TankAIController.h"
#include "DrawDebugHelpers.h"
#include "WaypointActor.h"
#include "Kismet/GameplayStatics.h"


void ATankAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	MyPawn = Cast<AEnemyTank>(InPawn);
	if (!MyPawn)
		return;
	
	GeneratePatrollingPoints();
}

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (MyPawn)
		GetClosestTarget();

	if (MyTarget)
		Aiming();

	if (PatrollingPoints.Num() > 0)
		MoveToNextPoint(DeltaTime);
}


void ATankAIController::GetClosestTarget()
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
		GetWorld()->LineTraceSingleByChannel(Hit, MyPawn->GetActorLocation(), TmpActor->GetActorLocation(), ECollisionChannel::ECC_Visibility, Params);

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

void ATankAIController::GeneratePatrollingPoints()
{
	TArray<AActor*> Actors;
	TArray<AWaypointActor*> Waypoints;
    
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaypointActor::StaticClass(), Actors);
	for (auto Actor : Actors)
	{
		if (Actor->Tags.Contains(MyPawn->WaypointTag))
			Waypoints.Add(Cast<AWaypointActor>(Actor));
	}
	Waypoints.Sort([] (AWaypointActor& a, AWaypointActor& b)
	{
		return a.WaypointIndex < b.WaypointIndex;
	});

	for (auto Waypoint : Waypoints)
	{
		FVector TmpPoint = Waypoint->GetActorLocation();
		TmpPoint.Z = MyPawn->GetActorLocation().Z;
		PatrollingPoints.Add(TmpPoint);
	}

	for (int id = 1; id < PatrollingPoints.Num(); id++)
	{
		DrawDebugLine(GetWorld(), PatrollingPoints[id-1], PatrollingPoints[id], FColor::Yellow, true);
	}
	DrawDebugLine(GetWorld(), PatrollingPoints[0], PatrollingPoints[PatrollingPoints.Num() - 1], FColor::Yellow, true);
}

void ATankAIController::Aiming() const
{
	MyPawn->RotateTurretToTarget(MyTarget->GetActorLocation(), false);
    
	if (MyPawn->CanFire(MyTarget->GetActorLocation()))
	{
		MyPawn->Fire();
	}
}


void ATankAIController::MoveToNextPoint(float DeltaTime)
{
	FVector PawnLocation = MyPawn->GetActorLocation();
	FVector CurrentPoint = PatrollingPoints[CurrentPatrolPointIndex];
	if (FVector::Distance(PawnLocation, CurrentPoint) <= MyPawn->GetMovementAccuracy())
	{
		CurrentPatrolPointIndex++;
		if (CurrentPatrolPointIndex >= PatrollingPoints.Num())
		{
			CurrentPatrolPointIndex = 0;
		}
	}
	   
	FVector MoveDirection = CurrentPoint - PawnLocation;
	MoveDirection.Normalize();
	   
	FVector ForwardDirection = MyPawn->GetActorForwardVector();
	FVector RightDirection = MyPawn->GetActorRightVector();
	       
	DrawDebugLine(GetWorld(), PawnLocation, CurrentPoint, FColor::Green, false, 0, 0, 10);
	   
	float ForwardAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ForwardDirection, MoveDirection)));
	float RightAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(RightDirection, MoveDirection)));
	   
	int RotationValue = 0;
	if (ForwardAngle > 5)
		RotationValue = 1;
	if (RightAngle > 90.f)
		RotationValue = -RotationValue;

	if (ForwardAngle > 45 )
		MyPawn->MoveForward(0);
	else
		MyPawn->MoveForward(1);

	MyPawn->RotateRight(RotationValue);

}

#pragma optimize( "", on )