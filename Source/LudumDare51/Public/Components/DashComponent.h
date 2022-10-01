// "Copyright (c) 2022. Made by Title Goose Team during LudumDare 51. All rights reserved. "

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DashComponent.generated.h"

class APlayerCharacter;
class UCharacterMovementComponent;
class APlayerController;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LUDUMDARE51_API UDashComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDashComponent();

	UFUNCTION(BlueprintCallable, BlueprintGetter, Category=Dash)
	bool GetIsDashing();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void Dash(const FVector& Direction);

	UFUNCTION(BlueprintGetter, Category=Dash)
	FTimerHandle GetCooldownHandle() const;

	UFUNCTION(BlueprintCallable, Category=Dash)
	bool IsOnCooldown();

protected:
	UPROPERTY()
	APlayerCharacter* Character = nullptr;

	UPROPERTY()
	UCharacterMovementComponent* MovementComponent = nullptr;

	UPROPERTY()
	APlayerController* PlayerController = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintGetter=GetIsDashing, Category=Dash)
	bool bIsDashing = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DashPower = 2700.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DashCooldown = 2.f;

	UPROPERTY(BlueprintGetter=GetCooldownHandle)
	FTimerHandle DashCooldownHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DashTime = .25f;

	UFUNCTION()
	void SetIsDashing(const bool bEnableDash);
	
};
