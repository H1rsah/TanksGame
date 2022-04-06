// Copyright Epic Games, Inc. All Rights Reserved.


#include "TanksGameGameModeBase.h"
#include "Controllers/TankPlayerController.h"
#include "Interfaaces/Scoreable.h"

void ATanksGameGameModeBase::NotifyActorWasDestroyedByDamage(AActor* Actor, FDamageTypes& DamageType)
{
	if (IScoreable* Scoreable = Cast<IScoreable>(Actor))
	{
		ATankPlayerController* PlayerController = Cast<ATankPlayerController>(GetWorld()->GetFirstPlayerController());
		if (DamageType.Instigator == PlayerController->GetPawn())
		{
			PlayerController->PlayerScore += Scoreable->GetScores();
		}
	}
}
