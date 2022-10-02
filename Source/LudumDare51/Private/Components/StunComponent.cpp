// "Copyright (c) 2022. Made by Title Goose Team during LudumDare 51. All rights reserved. "


#include "Components/StunComponent.h"

#include "GameFramework/Character.h"


UStunComponent::UStunComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStunComponent::ApplyStun(float Duration)
{
	if (IsStunned() || Duration <= 0.f) return;

	// Character->DisableInput(Cast<APlayerController>(Character->GetController()));
	GetWorld()->GetTimerManager().SetTimer(StunTimerHandle, this, &UStunComponent::EndStun, Duration, false);
	OnStunApplied.Broadcast();
}

void UStunComponent::EndStun() const
{
	// Character->EnableInput(Cast<APlayerController>(Character->GetController()));
	OnStunEnded.Broadcast();
}


bool UStunComponent::IsStunned() const
{
	return GetWorld()->GetTimerManager().IsTimerActive(StunTimerHandle);
}

void UStunComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UStunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

