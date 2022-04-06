// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaypointActor.generated.h"

UCLASS()
class TANKSGAME_API AWaypointActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AWaypointActor();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move")
	int32 WaypointIndex;
};
