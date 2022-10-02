// "Copyright (c) 2022. Made by Title Goose Team during LudumDare 51. All rights reserved. "

#pragma once

#include "CoreMinimal.h"
#include "Core/TrickyCoreTypes.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "Goose.generated.h"

class UInteractionSphereComponent;
class UAudioComponent;
class UParticleSystemComponent;

UCLASS()
class LUDUMDARE51_API AGoose : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	AGoose();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	USceneComponent* GooseRoot = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UStaticMeshComponent* GooseMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UInteractionSphereComponent* InteractionTrigger = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UAudioComponent* AudioComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UParticleSystemComponent* ParticleSystemComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<UStaticMesh*> GooseMeshes;

public:
	virtual void Tick(float DeltaTime) override;

private:
	virtual bool ProcessInteraction_Implementation(AActor* TargetActor) override;

	UFUNCTION()
	void OnSessionStateChange(const ESessionState NewState);

	void SetHonkingEnabled(const bool bIsEnabled);

	UFUNCTION()
	void PlayHonk();

	FTimerHandle HonkingTimerHandle;
	
	// Animation
private:
	FVector InitialLocation{FVector::ZeroVector};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation", meta=(AllowPrivateAccess="true"))
	bool bAnimateRotation = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation", meta=(AllowPrivateAccess="true"))
	float RotationSpeed = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation", meta=(AllowPrivateAccess="true"))
	bool bAnimatePosition = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation", meta=(AllowPrivateAccess="true"))
	float Amplitude = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation", meta=(AllowPrivateAccess="true"))
	float Frequency = 1.f;

	void AnimateRotation() const;
	void AnimatePosition() const;
};
