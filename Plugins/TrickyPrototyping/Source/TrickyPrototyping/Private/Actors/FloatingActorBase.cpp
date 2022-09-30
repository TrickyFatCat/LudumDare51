// Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Actors/FloatingActorBase.h"

#include "Components/TimelineComponent.h"
#include "Core/TrickyFunctionLibrary.h"

AFloatingActorBase::AFloatingActorBase()
{
	PrimaryActorTick.bCanEverTick = true;

	ActorRoot = CreateDefaultSubobject<USceneComponent>("ActorRoot");
	SetRootComponent(ActorRoot);

	MovementTimeline = CreateDefaultSubobject<UTimelineComponent>("MovementTimeline");
}

void AFloatingActorBase::BeginPlay()
{
	if (MovementTimeline && MovementAnimationCurve)
	{
		FOnTimelineFloat MovementProgress{};
		MovementProgress.BindUFunction(this, FName("MoveActor"));
		MovementTimeline->AddInterpFloat(MovementAnimationCurve, MovementProgress);

		FOnTimelineEvent MovementFinished{};
		MovementFinished.BindUFunction(this, FName("ContinueMovement"));
		MovementTimeline->SetTimelineFinishedFunc(MovementFinished);

		UTrickyFunctionLibrary::CalculateTimelinePlayRate(MovementTimeline, Cast<UCurveBase>(MovementAnimationCurve), TravelTime);
	}

	if (WaitDuration <= 0.f)
	{
		bStopAtPoints = false;
	}

	if (bAutoStart && MovementMode != EFloatingActorMovementMode::Manual)
	{
		StartMovement();
	}

	Super::BeginPlay();
}

void AFloatingActorBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ConstructActor();
}

void AFloatingActorBase::ConstructActor()
{
	FillPointIndexes();

	if (PointsIndexes.Num() > 0)
	{
		if (!IndexIsValid(StartPointIndex))
		{
			StartPointIndex = 0;
			LogError("Invalid start point index, it can't be < 0 or > the max index in the array.");
		}

		CurrentPointIndex = StartPointIndex;
		NextPointIndex = StartPointIndex;
		MoveActor(0.0f);

		if (bStopAtPoints && bStopAtCertainPoints && CustomStopsIndexes.Num() > 0)
		{
			RemoveInvalidCustomIndexes();

			// Sort custom indexes
			if (bSortCustomStops)
			{
				auto Iterator = [](const int32& Lhs, const int32& Rhs)-> bool { return Lhs <= Rhs; };
				CustomStopsIndexes.Sort(Iterator);
			}
		}
	}
	else
	{
		LogError("Couldn't construct the actor. There are no points to move between.");
	}
}

void AFloatingActorBase::StartMovement()
{
	if (State == EFloatingActorState::Moving || PointsIndexes.Num() == 0) return;

	if (CurrentPointIndex == NextPointIndex)
	{
		CalculateNextPointIndex();
	}

	CalculateTravelTime();
	UTrickyFunctionLibrary::CalculateTimelinePlayRate(MovementTimeline, Cast<UCurveBase>(MovementAnimationCurve), TravelTime);

	if (bStopAtPoints && bWaitAtStart)
	{
		StartStopWaitTimer();
		return;
	}

	MovementTimeline->PlayFromStart();
	SetState(EFloatingActorState::Moving);
}

void AFloatingActorBase::StopMovement()
{
	if (State == EFloatingActorState::Idle) return;

	MovementTimeline->Stop();
	SetState(EFloatingActorState::Idle);
}

void AFloatingActorBase::ResumeMovement()
{
	if (State != EFloatingActorState::Idle) return;

	MovementTimeline->Play();
	SetState(EFloatingActorState::Moving);
}

void AFloatingActorBase::MoveToPoint(const int32 PointIndex)
{
	if (!IndexIsValid(PointIndex) || MovementMode != EFloatingActorMovementMode::Manual) return;

	NextPointIndex = PointIndex;
	CalculateTravelTime();
	UTrickyFunctionLibrary::CalculateTimelinePlayRate(MovementTimeline, Cast<UCurveBase>(MovementAnimationCurve), TravelTime);
	SetState(EFloatingActorState::Moving);
	MovementTimeline->PlayFromStart();
}

void AFloatingActorBase::SetSpeed(const float Value)
{
	if (Value < 0.f) return;

	Speed = Value;
	CalculateTravelTime();
}

void AFloatingActorBase::SetTravelTime(const float Value)
{
	if (bUseTravelTime || Value <= 0.f) return;

	TravelTime = Value;
	UTrickyFunctionLibrary::CalculateTimelinePlayRate(MovementTimeline, Cast<UCurveBase>(MovementAnimationCurve), TravelTime);
}

void AFloatingActorBase::SetWaitDuration(const float Value)
{
	if (Value < 0.f) return;

	WaitDuration = Value;

	if (WaitDuration == 0.f)
	{
		bStopAtPoints = false;
	}
}

bool AFloatingActorBase::IndexIsValid(const int32 Index) const
{
	return Index >= 0 && Index < PointsIndexes.Num();
}

void AFloatingActorBase::CalculateTravelTime()
{
}

void AFloatingActorBase::FillPointIndexes()
{
	PointsIndexes.Empty();
}

void AFloatingActorBase::SortPointsIndexes()
{
	if (!bSortCustomStops) return;

	auto Iterator = [](const int32& Lhs, const int32& Rhs)-> bool { return Lhs <= Rhs; };
	PointsIndexes.Sort(Iterator);
}

void AFloatingActorBase::RemoveInvalidCustomIndexes()
{
}

void AFloatingActorBase::MoveActor(const float Progress)
{
}

void AFloatingActorBase::ContinueMovement()
{
	if (State == EFloatingActorState::Idle) return;

	OnPointReached.Broadcast(CurrentPointIndex);

	if (MovementMode == EFloatingActorMovementMode::Manual)
	{
		CurrentPointIndex = NextPointIndex;
		SetState(EFloatingActorState::Idle);
		return;
	}

	CalculateNextPointIndex();
	CalculateTravelTime();
	UTrickyFunctionLibrary::CalculateTimelinePlayRate(MovementTimeline, Cast<UCurveBase>(MovementAnimationCurve), TravelTime);

	if (bStopAtPoints)
	{
		StartStopWaitTimer();
	}
	else
	{
		MovementTimeline->PlayFromStart();
	}
}

void AFloatingActorBase::CalculateNextPointIndex()
{
}

void AFloatingActorBase::StartStopWaitTimer()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if (!TimerManager.IsTimerActive(WaitTimerHandle))
	{
		OnWaitStarted.Broadcast(CurrentPointIndex);
		SetState(EFloatingActorState::Waiting);
		TimerManager.SetTimer(WaitTimerHandle,
		                      this,
		                      &AFloatingActorBase::FinishStopTimer,
		                      WaitDuration,
		                      false);
	}
}

void AFloatingActorBase::FinishStopTimer()
{
	OnWaitFinished.Broadcast(CurrentPointIndex);
	SetState(EFloatingActorState::Moving);
	MovementTimeline->PlayFromStart();
}

void AFloatingActorBase::SetState(const EFloatingActorState NewState)
{
	if (State == NewState) return;

	State = NewState;
	OnStateChanged.Broadcast(State);
}

void AFloatingActorBase::LogError(const FString& Message) const
{
	if (!GetWorld()) return;

	const FString ErrorMessage{FString::Printf(TEXT("%s | Actor: %s"), *Message, *GetName())};
	UE_LOG(LogFloatingActor, Error, TEXT("%s"), *ErrorMessage);
}
