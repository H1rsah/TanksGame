// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

UENUM()
enum class EWidgetsEnum : uint8
{
	Wid_None UMETA(DisplayName = "/"), 
	Wid_MainMenu UMETA(DisplayName = "Main Menu"),
	Wid_InGame UMETA(DisplayName = "In Game"),
	Wid_GameOver UMETA(DisplayName = "GameOver"),
};

UCLASS()
class TANKSGAME_API AMyHUD : public AHUD
{
	GENERATED_BODY()

public:
	AMyHUD();
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	UUserWidget* ShowWidget(const EWidgetsEnum WidgetID, const int32 ZOrder = 0);
	UFUNCTION(BlueprintCallable)
	void HideWidget();
	UFUNCTION(BlueprintPure)
	UUserWidget* GetCurrentWidget() const { return CurrentWidget; }

protected:
	virtual void BeginPlay() override;

	UUserWidget* CreateWidgetByClass(const TSubclassOf<UUserWidget> WidgetClass, const int32 ZOrder = 0);
	UPROPERTY(EditAnywhere)
	TMap<EWidgetsEnum, TSubclassOf<UUserWidget>> WidgetClasses;
	UPROPERTY()
	UUserWidget* CurrentWidget;
};
