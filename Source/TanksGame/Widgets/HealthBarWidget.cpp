// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarWidget.h"

void UHealthBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!OwnerHealth.IsValid())
		return;

	HealthBar->SetPercent(OwnerHealth->CurrentHealth / OwnerHealth->MaxHealth);
}
