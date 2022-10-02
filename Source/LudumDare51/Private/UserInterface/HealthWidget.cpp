// Copyright (c) 2022. Made by Title Goose Team during LudumDare 51. All rights reserved. 


#include "UserInterface/HealthWidget.h"

#include "Components/HealthComponent.h"
#include "Components/HorizontalBox.h"
#include "Components/Spacer.h"

class USpacer;

void UHealthWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	CreateHealthIcons();
}

void UHealthWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	const APawn* PlayerPawn = GetOwningPlayerPawn();

	if (PlayerPawn)
	{
		HealthComponent = PlayerPawn->FindComponentByClass<UHealthComponent>();

		if (HealthComponent)
		{
			CreateHealthIcons(HealthComponent->GetMaxHealth());
		}
	}
}

void UHealthWidget::CreateHealthIcons(const int32 Amount)
{
	HBox_Health->ClearChildren();
	HealthIcons.Empty();

	if (!HealthIconWidget) return;

	for (int32 i = 0; i < Amount; i++)
	{
		UUserWidget* HealthIcon = CreateWidget(this, HealthIconWidget);

		if (!HealthIcon) continue;

		HBox_Health->AddChild(HealthIcon);
		HealthIcons.Add(HealthIcon);

		if (i < Amount - 1)
		{
			USpacer* Spacer = NewObject<USpacer>(this);
			Spacer->SetSize(FVector2D{8.f, 0.f});
			HBox_Health->AddChild(Spacer);
		}
	}
}
