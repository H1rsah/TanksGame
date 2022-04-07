// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TanksGame/Structures/GameStructures.h"
#include "TanksGameGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TANKSGAME_API ATanksGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	void NotifyActorWasDestroyedByDamage(AActor* Actor, const FDamageTypes& DamageType) const;
};
