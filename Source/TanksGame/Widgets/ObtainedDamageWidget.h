// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageNotifier.h"
#include "Components/Overlay.h"
#include "TanksGame/Structures/GameStructures.h"
#include "ObtainedDamageWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class TANKSGAME_API UObtainedDamageWidget : public UUserWidget
{
	GENERATED_BODY()

	// TArray<FDamageTypes> ObtainedDamageQueue;
	
protected:
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	// UOverlay* ObtainedDamageOverlay;
	
public:
	// void ObtainDamage(float Damage);
	UFUNCTION(BlueprintImplementableEvent)
	void Refresh();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageAmount;
	// UFUNCTION(BlueprintCallable)
	// void OnObtainedMessageEnd();
};
