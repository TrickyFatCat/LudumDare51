// Copyright (c) 2022. Made by Title Goose Team during LudumDare 51. All rights reserved. 


#include "UserInterface/DashIndicatorWidget.h"

#include "Components/DashComponent.h"


void UDashIndicatorWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	const APawn* PlayerCharacter = GetOwningPlayerPawn();
	DashComponent = PlayerCharacter->FindComponentByClass<UDashComponent>();
}

float UDashIndicatorWidget::GetCooldownProgress() const
{
	if (!DashComponent->IsOnCooldown())
	{
		return 1.f;
	}

	return GetWorld()->GetTimerManager().GetTimerElapsed(DashComponent->GetCooldownHandle()) / DashComponent->GetCooldownTime();
}
