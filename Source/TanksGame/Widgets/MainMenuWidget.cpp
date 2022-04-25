// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (NewGameBtn)
		NewGameBtn->OnPressed.AddDynamic(this, &ThisClass::OnNewGameClicked);

	if (ExitBtn)
		ExitBtn->OnPressed.AddDynamic(this, &ThisClass::OnExitClicked);
}

void UMainMenuWidget::NativeDestruct()
{
	if (NewGameBtn)
		NewGameBtn->OnPressed.RemoveAll(this);

	if (ExitBtn)
		ExitBtn->OnPressed.RemoveAll(this);

	Super::NativeDestruct();
}

void UMainMenuWidget::OnNewGameClicked()
{
	UGameplayStatics::OpenLevel(this, FName("Level02"), true);
}

void UMainMenuWidget::OnExitClicked()
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}
