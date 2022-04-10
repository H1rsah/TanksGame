// Copyright Epic Games, Inc. All Rights Reserved.


#include "TanksGameGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Controllers/PlayerTankController.h"
#include "Interfaaces/Scoreable.h"

// ATanksGameGameModeBase::ATanksGameGameModeBase()
// {
// 	UMGState = EUMGState::UIS_Ingame;
// }

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
//
// void ATanksGameGameModeBase::BeginPlay()
// {
// 	Super::BeginPlay();
//
// 	ApplyUMGChanges();
// }
//
// void ATanksGameGameModeBase::ApplyUMGChanges()
// {
// 	if(CurrentWidget)
// 		CurrentWidget->RemoveFromParent();
//
// 	switch(UMGState)
// 	{
// 	case UIS_Ingame:
// 		{
// 			ApplyUMG(IngameUMG, true, false);
// 			break;
// 		}
// 	case UIS_GameOver:
// 		{
// 			ApplyUMG(GameOverUMG, true, true);
// 			break;
// 		}
// 	default:
// 		{
// 			ApplyUMG(IngameUMG, false, false);
// 			break;
// 		}
// 	}
// }
//
// void ATanksGameGameModeBase::SetUMGState(uint8 NewState)
// {
// 	UMGState = NewState;
// 	ApplyUMGChanges();
// }
//
// bool ATanksGameGameModeBase::ApplyUMG(TSubclassOf<UUserWidget> WidgetToApply, bool bShowMouseCursor, bool bEnableClickEvents)
// {
// 	APlayerController* MyController = GetWorld()->GetFirstPlayerController();
//
// 	if(WidgetToApply)
// 	{
// 		// Set mouse settings
// 		MyController->bShowMouseCursor = bShowMouseCursor;
// 		MyController->bEnableClickEvents = bEnableClickEvents;
//
// 		// Create widget
// 		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetToApply);
// 		if(CurrentWidget)
// 		{
// 			CurrentWidget->AddToViewport();
// 			return true;
// 		}
// 	}
// 	return false;
// }

