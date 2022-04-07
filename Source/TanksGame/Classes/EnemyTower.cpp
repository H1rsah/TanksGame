// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyTower.h"

AEnemyTower::AEnemyTower()
{
	Mobility = EUnitMobility::Static;
}

void AEnemyTower::BeginPlay()
{
	Super::BeginPlay();

	SetAmmo(TowerAmmoAmount);
}
