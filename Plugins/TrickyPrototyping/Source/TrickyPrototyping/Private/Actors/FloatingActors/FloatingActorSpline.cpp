// Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Actors/FloatingActors/FloatingActorSpline.h"

#include "Components/SplineComponent.h"
#include "Core/TrickyUtils.h"

void AFloatingActorSpline::ConstructActor()
{
	if (!SplineActor)
	{
		LogError("SplineActor is nullptr. Please choose an actor with spline to use this floating actor.");
		return;
	}

	TargetSplineComponent = FTrickyUtils::GetComponent<USplineComponent>(SplineActor);

	Super::ConstructActor();
}

void AFloatingActorSpline::SetSplineActor(AActor* Value)
{
	if (!Value) return;

	SplineActor = Value;
	TargetSplineComponent = FTrickyUtils::GetComponent<USplineComponent>(Value);
}

void AFloatingActorSpline::CalculateTravelTime()
{
	Super::CalculateTravelTime();

	if (!TargetSplineComponent || bUseTravelTime) return;

	const float StartDistance = GetSplineDistance(CurrentPointIndex);
	const float FinishDistance = GetSplineDistance(NextPointIndex);
	const float DistanceBetweenPoints = FMath::Abs(FinishDistance - StartDistance);
	TravelTime = DistanceBetweenPoints / Speed;
}

void AFloatingActorSpline::FillPointIndexes()
{
	if (!TargetSplineComponent)
	{
		LogError("The target actor doesn't have a spline component.");
		return;
	}

	PointsIndexes.Empty();

	auto GetPointsFromSpline = [&]()-> void
	{
		for (int32 i = 0; i < TargetSplineComponent->GetNumberOfSplinePoints(); i++)
		{
			PointsIndexes.Add(i);
		}

		if (TargetSplineComponent->IsClosedLoop())
		{
			PointsIndexes.Add(TargetSplineComponent->GetNumberOfSplinePoints());
		}
	};

	switch (MovementMode)
	{
		case EFloatingActorMovementMode::Manual:
			GetPointsFromSpline();
			break;

		default:
			if (bStopAtPoints)
			{
				if (bStopAtCertainPoints)
				{
					if (CustomStopsIndexes.Num() < 2)
					{
						LogError("Not enough custom indexes. There must be at least 2.");
						return;
					}

					PointsIndexes = CustomStopsIndexes;
					SortPointsIndexes();

					return;
				}

				GetPointsFromSpline();
			}
			else
			{
				PointsIndexes.Add(0);
				const int PointsNumber = TargetSplineComponent->GetNumberOfSplinePoints();
				PointsIndexes.Add(TargetSplineComponent->IsClosedLoop() ? PointsNumber : PointsNumber - 1);
			}
			break;
	}
}

void AFloatingActorSpline::RemoveInvalidCustomIndexes()
{
	Super::RemoveInvalidCustomIndexes();

	for (int32 i = 0; i < CustomStopsIndexes.Num(); i++)
	{
		const int32 Index = CustomStopsIndexes[i];

		if (Index < 0)
		{
			CustomStopsIndexes.Remove(Index);
			continue;
		}

		if (!TargetSplineComponent)
		{
			continue;
		}

		const int32 NumberOfPoints = TargetSplineComponent->GetNumberOfSplinePoints();
		const bool bIsValid = TargetSplineComponent->IsClosedLoop() ? Index <= NumberOfPoints : Index < NumberOfPoints;

		if (!bIsValid)
		{
			CustomStopsIndexes.Remove(Index);
		}
	}
}

void AFloatingActorSpline::MoveActor(const float Progress)
{
	Super::MoveActor(Progress);

	if (!TargetSplineComponent) return;

	const FVector NewLocation{
		TargetSplineComponent->GetLocationAtDistanceAlongSpline(GetPositionAtSpline(Progress),
		                                                        ESplineCoordinateSpace::World)
	};
	SetActorLocation(NewLocation);
	RotateAlongSpline(Progress);
	ScaleAlongSpline(Progress);
}

void AFloatingActorSpline::CalculateNextPointIndex()
{
	auto CalculateNextIndex = [&]()-> void
	{
		NextPointIndex = bIsReversed ? CurrentPointIndex - 1 : CurrentPointIndex + 1;
	};

	CurrentPointIndex = NextPointIndex;
	CalculateNextIndex();
	const bool bIndexIsValid = IndexIsValid(NextPointIndex);

	switch (MovementMode)
	{
		case EFloatingActorMovementMode::Loop:
			if (!bIndexIsValid)
			{
				CurrentPointIndex = bIsReversed ? PointsIndexes.Num() - 1 : 0;
				CalculateNextIndex();
			}
			break;

		case EFloatingActorMovementMode::PingPong:
			if (!bIndexIsValid)
			{
				bIsReversed = !bIsReversed;
				CalculateNextIndex();
			}
			break;

		default:
			break;
	}
}

float AFloatingActorSpline::GetSplineDistance(const int32 PointIndex) const
{
	if (!TargetSplineComponent) return -1.f;

	return TargetSplineComponent->GetDistanceAlongSplineAtSplinePoint(PointsIndexes[PointIndex]);
}

float AFloatingActorSpline::GetPositionAtSpline(const float Progress) const
{
	if (!TargetSplineComponent) return -1;

	const float Start = GetSplineDistance(CurrentPointIndex);
	const float Finish = GetSplineDistance(NextPointIndex);

	return FMath::Lerp(Start, Finish, Progress);
}

void AFloatingActorSpline::RotateAlongSpline(const float Progress)
{
	if (!TargetSplineComponent) return;

	if (InheritSplineRotation.bInheritX || InheritSplineRotation.bInheritY || InheritSplineRotation.bInheritZ)
	{
		const FRotator CurrentRotation{GetActorRotation()};
		const FRotator RotationAlongSpline{
			TargetSplineComponent->GetRotationAtDistanceAlongSpline(GetPositionAtSpline(Progress),
			                                                        ESplineCoordinateSpace::World)
		};
		
		const float NewRoll = InheritSplineRotation.bInheritX ? RotationAlongSpline.Roll : CurrentRotation.Roll;
		const float NewPitch = InheritSplineRotation.bInheritY ? RotationAlongSpline.Pitch : CurrentRotation.Pitch;
		const float NewYaw = InheritSplineRotation.bInheritZ ? RotationAlongSpline.Yaw : CurrentRotation.Yaw;
		
		SetActorRotation(FRotator{NewPitch, NewYaw, NewRoll});
	}
}

void AFloatingActorSpline::ScaleAlongSpline(const float Progress)
{
	if (!TargetSplineComponent) return;

	if (InheritSplineScale.bInheritX || InheritSplineScale.bInheritY || InheritSplineScale.bInheritZ)
	{
		const FVector CurrentScale{GetActorScale3D()};
		const FVector ScaleAlongSpline{
			TargetSplineComponent->GetScaleAtDistanceAlongSpline(GetPositionAtSpline(Progress))
		};
		
		const float NewScaleX = InheritSplineScale.bInheritX ? ScaleAlongSpline.X : CurrentScale.X;
		const float NewScaleY = InheritSplineScale.bInheritY ? ScaleAlongSpline.Y : CurrentScale.Y;
		const float NewScaleZ = InheritSplineScale.bInheritZ ? ScaleAlongSpline.Z : CurrentScale.Z;
		
		SetActorScale3D(FVector{NewScaleX, NewScaleY, NewScaleZ});
	}
}
