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
	// ATanksGameGameModeBase();
	//
	// enum EUMGState : uint8
	// {
	// 	UIS_Ingame,
	// 	UIS_GameOver
	// };
	
	void NotifyActorWasDestroyedByDamage(AActor* Actor, const FDamageTypes& DamageType) const;

// 	// Checks the UMG State and call ApplyUMGChanges() according UMG we want
// 	void ApplyUMGChanges();
//
// 	// Getter for UMG state
// 	uint8 GetUMGState() const { return UMGState; };
//
// 	// Setter for UMG state
// 	UFUNCTION(BlueprintCallable, Category = "UMG Functions")
// 	void SetUMGState(uint8 NewState);
//
// 	// Apply UMG to the screen
// 	bool ApplyUMG(TSubclassOf<UUserWidget> WidgetToApply, bool bShowMouseCursor, bool bEnableClickEvents);
//
// 	UFUNCTION(BlueprintCallable, Category = "UMG Functions")
// 	UUserWidget* GetInGameWidget() {return IngameUMG.GetDefaultObject();}
//
// protected:
// 	virtual void BeginPlay() override;
//
// 	uint8 UMGState;
// 	
// 	// UMG by default in game
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UMG|Widgets", meta = (BlueprintProtected = "true"))
// 	TSubclassOf<UUserWidget> IngameUMG;
// 	// UMG Game Over
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UMG|Widgets", meta = (BlueprintProtected = "true"))
// 	TSubclassOf<UUserWidget> GameOverUMG;
//
// 	UPROPERTY()
// 	UUserWidget* CurrentWidget;
};
