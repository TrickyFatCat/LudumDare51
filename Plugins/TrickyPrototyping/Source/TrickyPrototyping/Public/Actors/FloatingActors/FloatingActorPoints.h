// Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Actors/FloatingActorBase.h"
#include "FloatingActorPoints.generated.h"

/**
 * An actor which moves between given actors.
 * It's good for simple things like elevators and simple floating platforms.
 */
UCLASS(BlueprintType, Blueprintable)
class TRICKYPROTOTYPING_API AFloatingActorPoints : public AFloatingActorBase
{
	GENERATED_BODY()

protected:
	virtual void ConstructActor() override;
	
	virtual void CalculateTravelTime() override;

	virtual void FillPointIndexes() override;

	virtual void RemoveInvalidCustomIndexes() override;

	virtual void MoveActor(const float Progress) override;

	virtual void CalculateNextPointIndex() override;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FloatingActor|Points", meta=(AllowPrivateAccess="true"))
	TArray<AActor*> TargetActors;

	AActor* GetTargetActor(const int32 PointIndex) const;
};
