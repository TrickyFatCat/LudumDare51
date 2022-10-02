// "Copyright (c) 2022. Made by Title Goose Team during LudumDare 51. All rights reserved. "

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StunComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStunAppliedSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStunEndedSignature);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LUDUMDARE51_API UStunComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStunComponent();

	UFUNCTION(BlueprintCallable, Category=Stun)
	void ApplyStun(float Duration);

	UFUNCTION(BlueprintCallable, Category=Stun)
	bool IsStunned() const;

	UPROPERTY(BlueprintAssignable, Category=Stun)
	FOnStunAppliedSignature OnStunApplied;

	UPROPERTY(BlueprintAssignable, Category=Stun)
	FOnStunEndedSignature OnStunEnded;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	ACharacter* Character = nullptr;

	FTimerHandle StunTimerHandle;

	UFUNCTION()
	void EndStun() const;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
