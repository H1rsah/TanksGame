// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/Actor.h"
#include "LevelTrigger.generated.h"

class UBoxComponent;
UCLASS()
class TANKSGAME_API ALevelTrigger : public AActor
{
	GENERATED_BODY()
	
	ALevelTrigger();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void OnOverlapTrigger(UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int I, bool bArg, const FHitResult& HitResult);
	
public:
	// Sets default values for this actor's properties

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* TriggerBox;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USpotLightComponent* SpotLight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USpotLightComponent* InActiveLight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USpotLightComponent* ActiveLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	FName LevelName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Level")
	bool bIsActive = false;

	void ChangeFlag(bool IsActive);
};
