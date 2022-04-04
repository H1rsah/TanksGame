// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorPool.h"

DEFINE_LOG_CATEGORY_CLASS(UActorPool, LogActorPool);

AActor* UActorPool::RetreiveActor(UClass* Class, const FTransform& Transform)
{
    if (!Class)
    {
        return nullptr;
    }

    TArray<AActor*>* Pool = InactiveActors.Find(Class);
    AActor* Result = nullptr;
    if (Pool && Pool->Num() > 0)
    {
        Result = Pool->Pop();
        FVector CorrectedScale = Transform.GetScale3D() * Result->GetActorScale();
        Result->SetActorTransform(FTransform(Transform.GetRotation(), Transform.GetLocation(), CorrectedScale), false, nullptr, ETeleportType::ResetPhysics);
        Result->SetActorTickEnabled(true);
    }
    else
    {
        Result = GetWorld()->SpawnActor(Class, &Transform);
        if (!Result)
        {
            return nullptr;
        }

        Actors.Add(Result);
        Result->OnDestroyed.AddDynamic(this, &UActorPool::OnActorDestroyed);
    }

    return Result;
}

void UActorPool::ReturnActor(AActor* Actor)
{
    if (!Actor || !IsActorInPool(Actor))
    {
        return;
    }

    TArray<AActor*>& Pool = InactiveActors.FindOrAdd(Actor->GetClass());
    if (!Pool.Contains(Actor))
    {
        Actor->SetActorTickEnabled(false);
        Pool.Add(Actor);
    }
}

bool UActorPool::IsActorInPool(AActor* Actor) const
{
    return Actors.Contains(Actor);
}

bool UActorPool::DoesSupportWorldType(EWorldType::Type WorldType) const
{
    return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void UActorPool::OnActorDestroyed(AActor* Actor)
{
    check(IsActorInPool(Actor));
    TArray<AActor*>* Pool = InactiveActors.Find(Actor->GetClass());
    if (Pool)
    {
        Pool->Remove(Actor);
    }

    Actors.Remove(Actor);
}