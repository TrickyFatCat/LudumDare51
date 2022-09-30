// Copyright (c) 2021 Artyom "Tricky Fat Cat" Volkov (tricky.fat.cat@gmail.com)


#include "Components/TriggerComponents/BaseSphereTriggerComponent.h"

UBaseSphereTriggerComponent::UBaseSphereTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetCollisionObjectType(ECC_WorldStatic);
	SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void UBaseSphereTriggerComponent::BeginPlay()
{
	OnComponentBeginOverlap.AddDynamic(this, &UBaseSphereTriggerComponent::OnBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UBaseSphereTriggerComponent::OnEndOverlap);
	bIsEnabled ? EnableTrigger() : DisableTrigger();
	
	Super::BeginPlay();
}

void UBaseSphereTriggerComponent::SetIsEnabled(const bool bEnabled)
{
	if (bIsEnabled == bEnabled) return;

	bIsEnabled = bEnabled;
	bIsEnabled ? EnableTrigger() : DisableTrigger();
}

void UBaseSphereTriggerComponent::EnableTrigger_Implementation()
{
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UBaseSphereTriggerComponent::DisableTrigger_Implementation()
{
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UBaseSphereTriggerComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                                 AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComp,
                                                 int32 OtherBodyIndex,
                                                 bool bFromSweep,
                                                 const FHitResult& SweepResult)
{
	bIsActorInside = true;
}

void UBaseSphereTriggerComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                               AActor* OtherActor,
                                               UPrimitiveComponent* OtherComp,
                                               int32 OtherBodyIndex)
{
	bIsActorInside = false;
}
