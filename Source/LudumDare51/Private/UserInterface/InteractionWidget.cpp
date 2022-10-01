// Copyright (c) 2022. Made by Title Goose Team during LudumDare 51. All rights reserved. 


#include "UserInterface/InteractionWidget.h"

#include "Components/InteractionQueueComponent.h"

void UInteractionWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerPawn = GetOwningPlayerPawn();

	if (PlayerPawn)
	{
		InteractionComponent = PlayerPawn->FindComponentByClass<UInteractionQueueComponent>();
	}
}


