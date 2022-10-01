// Copyright (c) 2022. Made by Title Goose Team during LudumDare 51. All rights reserved. 


#include "GameplayObjects/DeathZone.h"

#include "Components/HealthComponent.h"
#include "Components/TriggerComponents/BaseBoxTriggerComponent.h"

ADeathZone::ADeathZone()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	Trigger = CreateDefaultSubobject<UBaseBoxTriggerComponent>("DeathTrigger");
	Trigger->SetupAttachment(GetRootComponent());
}

void ADeathZone::BeginPlay()
{
	Super::BeginPlay();

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ADeathZone::OnTriggerBeginOverlap);
}

void ADeathZone::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                       const FHitResult& SweepResult)
{
	UHealthComponent* HealthComponent = OtherActor->FindComponentByClass<UHealthComponent>();

	if (!HealthComponent) return;

	HealthComponent->ApplyDamage(HealthComponent->GetMaxHealth());
}

void ADeathZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
