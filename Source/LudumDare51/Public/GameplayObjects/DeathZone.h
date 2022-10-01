// Copyright (c) 2022. Made by Title Goose Team during LudumDare 51. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DeathZone.generated.h"

class UBaseBoxTriggerComponent;
class USceneComponent;

UCLASS()
class LUDUMDARE51_API ADeathZone : public AActor
{
	GENERATED_BODY()
	
public:	
	ADeathZone();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Components)
	UBaseBoxTriggerComponent* Trigger = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Components)
	USceneComponent* Root = nullptr;

	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	                           AActor* OtherActor,
	                           UPrimitiveComponent* OtherComp,
	                           int32 OtherBodyIndex,
	                           bool bFromSweep,
	                           const FHitResult& SweepResult);
	
public:	
	virtual void Tick(float DeltaTime) override;
};
