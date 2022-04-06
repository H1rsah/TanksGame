// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"

#include "TankPawn.h"
#include "DrawDebugHelpers.h"
#include "WaypointActor.h"
#include "Kismet/GameplayStatics.h"

void AEnemyController::BeginPlay()
{
    Super::BeginPlay();
}

void AEnemyController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
   
    if (TankPawn)
    {
        Targeting();
        MoveToNextPoint();
    }
}

void AEnemyController::MoveToNextPoint()
{
    if (PatrollingPoints.Num() == 0)
    {
        return;
    }

    FVector PawnLocation = TankPawn->GetActorLocation();
    FVector CurrentPoint = PatrollingPoints[CurrentPatrolPointIndex];
    if (FVector::Distance(PawnLocation, CurrentPoint) <= TankPawn->GetMovementAccuracy())
    {
        CurrentPatrolPointIndex++;
        if (CurrentPatrolPointIndex >= PatrollingPoints.Num())
        {
            CurrentPatrolPointIndex = 0;
        }
    }
    
    FVector MoveDirection = CurrentPoint - PawnLocation;
    MoveDirection.Normalize();
    
    FVector ForwardDirection = TankPawn->GetActorForwardVector();
    FVector RightDirection = TankPawn->GetActorRightVector();
        
    DrawDebugLine(GetWorld(), PawnLocation, CurrentPoint, FColor::Green, false, 0, 0, 1);
    
    float ForwardAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ForwardDirection, MoveDirection)));
    float RightAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(RightDirection, MoveDirection)));
    
    int RotationValue = 0;
    if (ForwardAngle > 5)
        RotationValue = 1;
    if (RightAngle > 90.f)
        RotationValue = -RotationValue;
    
    if (ForwardAngle > 30 )
        TankPawn->MoveForward(0);
    else
        TankPawn->MoveForward(1);

    TankPawn->RotateRight(RotationValue);
}

void AEnemyController::Targeting()
{
    APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
    if (FVector::DistSquared(PlayerPawn->GetActorLocation(), TankPawn->GetActorLocation()) > FMath::Square(TargetingRange))
    {
        return;
    }

    FHitResult HitResult;
    FVector TraceStart = TankPawn->GetActorLocation();
    FVector TraceEnd = PlayerPawn->GetActorLocation();
    FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("AI Vission Trace")), true, this);
    TraceParams.bReturnPhysicalMaterial = false;
    
    if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, TraceParams))
    {
        DrawDebugLine(GetWorld(), TraceStart, HitResult.Location, FColor::Red, false, 0, 0, 1);
        if (HitResult.Actor != PlayerPawn)
        {
            return;
        }
    }
    else
    {
        DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 0.1f, 0, 5);
    }

    TankPawn->SetTurretTargetPosition(PlayerPawn->GetActorLocation());
    
    FVector TargetingDir = TankPawn->GetTurretForwardVector();
    FVector DirToPlayer = PlayerPawn->GetActorLocation() - TankPawn->GetActorLocation();
    DirToPlayer.Normalize();
    float AimAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(TargetingDir, DirToPlayer)));
    
    if (AimAngle <= Accuracy)
    {
        TankPawn->Fire();
    }
}

void AEnemyController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    TankPawn = Cast<ATankPawn>(InPawn);
    if (!TankPawn)
        return;

    TArray<AActor*> Actors;
    TArray<AWaypointActor*> Waypoints;
    
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaypointActor::StaticClass(), Actors);
    for (auto Actor : Actors)
    {
        auto Waypoint = CastChecked<AWaypointActor>(Actor);
        if (Waypoint->Tags.Contains(TankPawn->WaypointTag))
            Waypoints.Add(Waypoint);
    }
    Waypoints.Sort([] (AWaypointActor& a, AWaypointActor& b)
    {
        return a.WaypointIndex > b.WaypointIndex;
    });

    for (auto W : Waypoints)
    {
        PatrollingPoints.Add(W->GetActorLocation());
    }

    for (int id = 1; id < PatrollingPoints.Num(); id++)
    {
        DrawDebugLine(GetWorld(), PatrollingPoints[id-1], PatrollingPoints[id],FColor::Yellow, true);
    }
    DrawDebugLine(GetWorld(), PatrollingPoints[0], PatrollingPoints[PatrollingPoints.Num()-1],FColor::Yellow, true);

}

void AEnemyController::AddTarget(AActor* Target)
{
}

void AEnemyController::RemoveTarget(AActor* Target)
{
}
