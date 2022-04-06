// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitBase.h"
#include "EnemyTower.generated.h"

/**
 * 
 */
UCLASS()
class TANKSGAME_API AEnemyTower : public AUnitBase
{
	GENERATED_BODY()
	
	AEnemyTower();
	virtual void BeginPlay() override;
};
