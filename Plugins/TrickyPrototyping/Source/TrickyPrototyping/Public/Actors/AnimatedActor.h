// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AnimatedActor.generated.h"

class UCurveFloat;
class UTimelineComponent;

/**
 * This is a base class for interactive objects such as doors, buttons, etc.
 * It contains the basic logic for controlling animation for this objects.
 */

UENUM(BlueprintType)
enum class EAnimatedActorState : uint8
{
	Opened,
	Closed,
	Locked,
	Disabled,
	Transition UMETA(Hidden)
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAnimatedActorChangedStateSignature, EAnimatedActorState, NewState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTransitionStartedSignature, EAnimatedActorState, TargetState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTransitionReversedSignature, EAnimatedActorState, TargetState);

UCLASS()
class TRICKYPROTOTYPING_API AAnimatedActor : public AActor
{
	GENERATED_BODY()

public:
	AAnimatedActor();

protected:
	virtual void BeginPlay() override;

	// Animation
public:
	UFUNCTION(BlueprintGetter, Category="AnimatedActor|Animation")
	float GetAnimationDuration() const { return AnimationDuration; }

	UFUNCTION(BlueprintSetter, Category="AnimatedActor|Animation")
	void SetAnimationDuration(const float Value);

	UFUNCTION(BlueprintGetter, Category="AnimatedActor|Animation")
	bool GetIsReversible() const { return bIsReversible; }

	UFUNCTION(BlueprintSetter, Category="AnimatedActor|Animation")
	void SetIsReversible(const bool Value) { bIsReversible = Value; }

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components")
	UTimelineComponent* AnimationTimeline = nullptr;
	
	UFUNCTION(BlueprintCallable, Category="AnimatedActor|Animation")
	void AddAnimatedComponent(USceneComponent* NewComponent);

	UFUNCTION(BlueprintCallable, Category="AnimatedActor|Animation")
	void FillAnimatedComponents(TArray<USceneComponent*> Components);

	void FillInitialTransforms();
	
	void SetInitialTransform();

	virtual void StartAnimation();

	virtual void ReverseAnimation();

	virtual void StopAnimation();

	UFUNCTION(BlueprintNativeEvent, Category="AnimatedActor|Animation")
	void FinishAnimation();

	virtual void FinishAnimation_Implementation();

	/**
	 * It's highly recommended to use 1 second long curves.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimatedActor|Animation", meta=(AllowPrivateAccess="true"))
	UCurveFloat* AnimationCurve = nullptr;

	UPROPERTY(VisibleDefaultsOnly,
		BlueprintReadOnly,
		Category="AnimatedActor|Animation",
		meta=(AllowPrivateAccess="true"))
	TArray<USceneComponent*> AnimatedComponents;

	UPROPERTY()
	TArray<FTransform> InitialTransforms;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimatedActor|Animation", meta=(AllowPrivateAccess="true"))
	TArray<FTransform> TransformOffsets;

	/**
	 * Determine how long the animation lasts in seconds.
	 */
	UPROPERTY(EditAnywhere,
		BlueprintGetter=GetAnimationDuration,
		BlueprintSetter=SetAnimationDuration,
		Category="AnimatedActor|Animation",
		meta=(AllowPrivateAccess="true"))
	float AnimationDuration = 1.f;

	/**
	 * Determine if the animation can be reversed during transition state.
	 */
	UPROPERTY(EditAnywhere,
		BlueprintGetter=GetIsReversible,
		BlueprintSetter=SetIsReversible,
		Category="AnimatedActor|Animation",
		meta=(AllowPrivateAccess="true"))
	bool bIsReversible = false;

	UFUNCTION()
	void AnimateTransform(const float AnimationProgress);

	bool CanBeReversed() const;

	// States
public:
	UFUNCTION(BlueprintGetter, Category="AnimatedActor|States")
	EAnimatedActorState GetStateCurrent() const { return StateCurrent; }

	UFUNCTION(BlueprintGetter, Category="AnimatedActor|States")
	EAnimatedActorState GetStateTarget() const { return StateTarget; }

	UFUNCTION(BlueprintGetter, Category="AnimatedActor|States")
	EAnimatedActorState GetStatePrevious() const { return StatePrevious; }

	UPROPERTY(BlueprintAssignable, Category="AnimatedActor|States")
	FOnAnimatedActorChangedStateSignature OnActorChangedState;

	UPROPERTY(BlueprintAssignable, Category="AnimatedActor|States")
	FOnTransitionStartedSignature OnActorTransitionStarted;

	UPROPERTY(BlueprintAssignable, Category="AnimatedActor|States")
	FOnTransitionReversedSignature OnActorTransitionReversed;

	UFUNCTION(BlueprintCallable, Category="AnimatedActor|States")
	bool IsStateCurrent(const EAnimatedActorState DoorState) const { return StateCurrent == DoorState; }

	UFUNCTION(BlueprintCallable, Category="AnimatedActor|States")
	bool IsStatePrevious(const EAnimatedActorState DoorState) const { return StatePrevious == DoorState; }

	UFUNCTION(BlueprintCallable, Category="AnimatedActor|States")
	bool IsStateTarget(const EAnimatedActorState DoorState) const { return StateTarget == DoorState; }

	UFUNCTION(BlueprintImplementableEvent, Category="AnimatedActor|States")
	void OnStateChanged(const EAnimatedActorState NewState);

	UFUNCTION(BlueprintImplementableEvent, Category="AnimatedActor|States")
	void OnTransitionStarted();

	UFUNCTION(BlueprintImplementableEvent, Category="AnimatedActor|States")
	void OnTransitionReversed();

protected:
	UFUNCTION(BlueprintCallable, Category="AnimatedActor|States")
	void SetState(const EAnimatedActorState NewState);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimatedActor|States", meta=(AllowPrivateAccess="true"))
	EAnimatedActorState StateInitial = EAnimatedActorState::Closed;

	UPROPERTY(VisibleAnywhere,
		BlueprintGetter=GetStateCurrent,
		Category="AnimatedActor|States",
		meta=(AllowPrivateAccess="true"))
	EAnimatedActorState StateCurrent = EAnimatedActorState::Closed;

	UPROPERTY(VisibleAnywhere,
		BlueprintGetter=GetStatePrevious,
		Category="AnimatedActor|States",
		meta=(AllowPrivateAccess="true"))
	EAnimatedActorState StatePrevious = EAnimatedActorState::Closed;

	UPROPERTY(VisibleAnywhere,
		BlueprintGetter=GetStateTarget,
		Category="AnimatedActor|States",
		meta=(AllowPrivateAccess="true"))
	EAnimatedActorState StateTarget = EAnimatedActorState::Opened;

	void SetTargetState();

	bool CanStartAnimation() const;

	// Actions
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="AnimatedActor|Actions")
	void Open();

	virtual void Open_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="AnimatedActor|Actions")
	void Close();

	virtual void Close_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="AnimatedActor|Actions")
	void Lock();

	virtual void Lock_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="AnimatedActor|Actions")
	void Unlock();

	virtual void Unlock_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="AnimatedActor|Actions")
	void Enable();

	virtual void Enable_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="AnimatedActor|Actions")
	void Disable();

	virtual void Disable_Implementation();

protected:
private:
};
