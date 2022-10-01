// "Copyright (c) 2022. Made by Title Goose Team during LudumDare 51. All rights reserved. "


#include "Components/HealthComponent.h"


UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


bool UHealthComponent::ApplyDamage(const int32 Amount)
{
	if (Amount <= 0 || bIsInvulnerable) return false;

	CurrentHealth -= Amount;
	CurrentHealth = FMath::Max(0, CurrentHealth);

	if (IsDead())
	{
		OnDeath.Broadcast();
	}
	else
	{
		OnDamageTaken.Broadcast(Amount, CurrentHealth);
	}

	return true;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

