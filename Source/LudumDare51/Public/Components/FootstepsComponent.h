// Copyright (c) 2022. Made by Title Goose Team during LudumDare 51. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "FootstepsComponent.generated.h"

/**
 * 
 */
UCLASS()
class LUDUMDARE51_API UFootstepsComponent : public UAudioComponent
{
	GENERATED_BODY()
	
public:	
	UFootstepsComponent();

	UFUNCTION(BlueprintCallable, Category="Sound|Footsteps")
	void StartPlayingSound();

	UFUNCTION(BlueprintCallable, Category="Sound|Footsteps")
	void StopPlayingSound();

	UFUNCTION(BlueprintCallable, Category="Sound|Footsteps")
	bool CanPlaySound() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Sound|Footsteps")
	float TimeBetweenFootsteps = 0.1f;
	
	FTimerHandle SoundPauseHandle;

	UFUNCTION()
	void PlaySound();
};
