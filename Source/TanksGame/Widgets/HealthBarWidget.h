// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "TanksGame/Components/HealthComponent.h"
#include "HealthBarWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TANKSGAME_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	TWeakObjectPtr<UHealthComponent> OwnerHealth;
	
protected:
	UPROPERTY(meta=(BindWidget))
	UProgressBar* HealthBar;

public:
	void SetOwnerUnit(UHealthComponent* InComponent) { OwnerHealth = InComponent; }
};
