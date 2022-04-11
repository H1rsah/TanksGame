// Copyright Epic Games, Inc. All Rights Reserved.


#include "TanksGameGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Controllers/PlayerTankController.h"
#include "Interfaaces/Scoreable.h"

void ATanksGameGameModeBase::NotifyActorWasDestroyedByDamage(AActor* Actor, const FDamageTypes& DamageType) const
{
	if (IScoreable* Scoreable = Cast<IScoreable>(Actor))
	{
		if (APlayerTankController* PlayerController = Cast<APlayerTankController>(GetWorld()->GetFirstPlayerController()))
			if (DamageType.Instigator == PlayerController->GetPawn())
			{
				PlayerController->PlayerScore += Scoreable->GetScores();
			}
	}
}