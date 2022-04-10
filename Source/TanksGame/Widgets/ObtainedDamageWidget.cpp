// Fill out your copyright notice in the Description page of Project Settings.


#include "ObtainedDamageWidget.h"


void UObtainedDamageWidget::OnObtainedMessageEnd()
{
	ObtainedDamageQueue.RemoveAt(0);
	ObtainedDamageOverlay->RemoveChildAt(0);
}
