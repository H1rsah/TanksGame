// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTrigger.h"

#include "MovieSceneSequenceID.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


ALevelTrigger::ALevelTrigger()
{
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>("TriggerBox");
	RootComponent = TriggerBox;
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ALevelTrigger::OnOverlapTrigger);

	InActiveLight = CreateDefaultSubobject<USpotLightComponent>("InActiveLight");
	InActiveLight->SetupAttachment(RootComponent);
	
	ActiveLight = CreateDefaultSubobject<USpotLightComponent>("ActiveLight");
	ActiveLight->SetupAttachment(RootComponent);
}

void ALevelTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALevelTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALevelTrigger::OnOverlapTrigger(UPrimitiveComponent* Component, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int I, bool bArg, const FHitResult& HitResult)
{
	if (bIsActive && !LevelName.IsNone())
		UGameplayStatics::OpenLevel(this, LevelName);
}

void ALevelTrigger::ChangeFlag(bool IsActive)
{
	InActiveLight->SetHiddenInGame(IsActive);
	ActiveLight->SetHiddenInGame(!IsActive);
	bIsActive = IsActive;
}

