// Copyright (c) 2022. Made by Title Goose Team during LudumDare 51. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class UInteractionQueueComponent;
class UKeyRingComponent;
class UDashComponent;

UCLASS()
class LUDUMDARE51_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Components)
	UCameraComponent* CameraComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UInteractionQueueComponent* InteractionQueueComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UKeyRingComponent* KeyRingComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UDashComponent* DashComponent = nullptr;
	
	void MoveForward(float AxisValue);

	void MoveRight(float AxisValue);

	void Interact();

	void Dash();

	virtual void Landed(const FHitResult& Hit) override;

	void ToggleCrouch(const bool bIsCrouching, const bool bForceCrouch = false);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess="true"))
	float CameraTransitionSpeed = 0.15f;
	FVector CameraInitialLocation = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	bool bWantCrouch = false;

	FVector LateralDirection = FVector::ZeroVector;

};
