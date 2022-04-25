// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MainMenuWidget.generated.h"

UCLASS(Abstract)
class TANKSGAME_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	UButton* NewGameBtn;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	UButton* ExitBtn;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* BlendIn;

	UFUNCTION()
	void OnNewGameClicked();

	UFUNCTION()
	void OnExitClicked();

};