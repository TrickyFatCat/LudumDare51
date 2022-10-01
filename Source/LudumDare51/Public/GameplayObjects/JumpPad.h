// Copyright (c) 2022. Made by Title Goose Team during LudumDare 51. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JumpPad.generated.h"

class UBaseSphereTriggerComponent;
class USoundCue;
class UArrowComponent;

UCLASS()
class LUDUMDARE51_API AJumpPad : public AActor
{
	GENERATED_BODY()
	
public:	
	AJumpPad();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	USceneComponent* Root = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UBaseSphereTriggerComponent* JumpTrigger = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* Mesh = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components")
	UArrowComponent* JumpDirection = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="JumpPad")
	float JumpForce = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="JumpPad")
	USoundCue* JumpSound = nullptr;

	UFUNCTION()
	virtual void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
									   AActor* OtherActor,
									   UPrimitiveComponent* OtherComp,
									   int32 OtherBodyIndex,
									   bool bFromSweep,
									   const FHitResult& SweepResult);

};
