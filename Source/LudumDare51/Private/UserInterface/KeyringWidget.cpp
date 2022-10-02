// Copyright (c) 2022. Made by Title Goose Team during LudumDare 51. All rights reserved. 


#include "UserInterface/KeyringWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBox.h"
#include "Components/KeyRingComponent.h"
#include "Objects/KeyType.h"
#include "Components/Spacer.h"

void UKeyringWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	CreateKeyIcons();
}

void UKeyringWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	const APawn* PlayerCharacter = GetOwningPlayerPawn();

	if (PlayerCharacter)
	{
		KeyRingComponent = PlayerCharacter->FindComponentByClass<UKeyRingComponent>();
	}
}

UUserWidget* UKeyringWidget::AddKeyIcon(TSubclassOf<UKeyType> KeyType, const bool bAddSpacer)
{
	UUserWidget* KeyIcon = CreateWidget(this, KeyIconWidget);

	HBox_Keys->AddChild(KeyIcon);
	KeyIcons.Add(KeyType, KeyIcon);

	if (bAddSpacer)
	{
		USpacer* Spacer = NewObject<USpacer>(this);
		Spacer->SetSize(FVector2D{8.f, 0.f});
		HBox_Keys->AddChild(Spacer);
	}

	return KeyIcon;
}

void UKeyringWidget::CreateKeyIcons()
{
	HBox_Keys->ClearChildren();
	KeyIcons.Empty();
	
	if (KeysSet.Array().Num() > 0 && KeyIconWidget)
	{
		for (int32 i = 0; i <= KeysSet.Array().Num()-1; i++)
		{
			const auto KeySubclass = KeysSet.Array()[i];
			UUserWidget* KeyIcon = AddKeyIcon(KeySubclass, i < KeysSet.Array().Num() - 1);
			if (!KeyIcon) continue;
			KeyIcon->SetColorAndOpacity(Cast<UKeyType>(KeySubclass->GetDefaultObject())->GetKeyColor());
		}
	}
}
