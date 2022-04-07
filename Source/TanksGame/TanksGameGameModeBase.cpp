// Copyright Epic Games, Inc. All Rights Reserved.


#include "TanksGameGameModeBase.h"
#include "Controllers/PlayerTankController.h"
#include "Interfaaces/Scoreable.h"

void ATanksGameGameModeBase::NotifyActorWasDestroyedByDamage(AActor* Actor, FDamageTypes& DamageType)
{
	if (IScoreable* Scoreable = Cast<IScoreable>(Actor))
	{
		APlayerTankController* PlayerController = Cast<APlayerTankController>(GetWorld()->GetFirstPlayerController());
		if (DamageType.Instigator == PlayerController->GetPawn())
		{
			PlayerController->PlayerScore += Scoreable->GetScores();
		}
	}
}
