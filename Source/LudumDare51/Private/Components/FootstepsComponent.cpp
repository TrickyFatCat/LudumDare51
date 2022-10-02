// Copyright (c) 2022. Made by Title Goose Team during LudumDare 51. All rights reserved. 


#include "Components/FootstepsComponent.h"

UFootstepsComponent::UFootstepsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetComponentTickInterval(0.15);
}

void UFootstepsComponent::StartPlayingSound()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if (TimerManager.IsTimerActive(SoundPauseHandle)) return;

	TimerManager.SetTimer(SoundPauseHandle,
	                      this,
	                      &UFootstepsComponent::PlaySound,
	                      TimeBetweenFootsteps,
	                      true);
}

void UFootstepsComponent::StopPlayingSound()
{
	if (!GetWorld() || !GetWorld()->GetTimerManager().IsTimerActive(SoundPauseHandle)) return;

	GetWorld()->GetTimerManager().ClearTimer(SoundPauseHandle);
}

bool UFootstepsComponent::CanPlaySound() const
{
	return !GetWorld()->GetTimerManager().IsTimerActive(SoundPauseHandle);
}

void UFootstepsComponent::PlaySound()
{
	if (IsPlaying()) return;
	Play();
}
