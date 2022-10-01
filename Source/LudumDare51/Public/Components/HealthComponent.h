// "Copyright (c) 2022. Made by Title Goose Team during LudumDare 51. All rights reserved. "

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDamageTakenSignature, int32, Amount, int32, NewHealth);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LUDUMDARE51_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();

	UFUNCTION(BlueprintGetter, Category=Health)
	int32 GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintGetter, Category=Health)
	int32 GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintCallable, Category=Health)
	bool IsDead() const { return CurrentHealth <= 0; }

	UFUNCTION(BlueprintCallable, Category=Health)
	bool ApplyDamage(const int32 Amount);

	UFUNCTION(BlueprintGetter, Category=Health)
	bool GetIsInvulnerable() const { return bIsInvulnerable; }

	UPROPERTY(BlueprintAssignable, Category=Health)
	FOnDeathSignature OnDeath;

	UPROPERTY(BlueprintAssignable, Category=Health)
	FOnDamageTakenSignature OnDamageTaken;
	

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintGetter=GetMaxHealth, Category=Health)
	int32 MaxHealth = 5;

	UPROPERTY(BlueprintGetter=GetCurrentHealth, Category=Health)
	int32 CurrentHealth = MaxHealth;

	UPROPERTY(BlueprintGetter=GetIsInvulnerable, Category=Health)
	bool bIsInvulnerable = false;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
