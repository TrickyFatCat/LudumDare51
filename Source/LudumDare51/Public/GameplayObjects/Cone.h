// "Copyright (c) 2022. Made by Title Goose Team during LudumDare 51. All rights reserved. "

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cone.generated.h"

class UBaseCapsuleTriggerComponent;

UCLASS()
class LUDUMDARE51_API ACone : public AActor
{
	GENERATED_BODY()

public:
	ACone();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float StunDuration = 0.75f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Components)
	USceneComponent* ConeRoot = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UStaticMeshComponent* ConeMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UBaseCapsuleTriggerComponent* DamageTrigger = nullptr;

	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	                           AActor* OtherActor,
	                           UPrimitiveComponent* OtherComp,
	                           int32 OtherBodyIndex,
	                           bool bFromSweep,
	                           const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LaunchPower = 1000.f;

public:
	virtual void Tick(float DeltaTime) override;
};
