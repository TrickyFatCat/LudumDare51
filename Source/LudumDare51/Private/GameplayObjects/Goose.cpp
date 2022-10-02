// "Copyright (c) 2022. Made by Title Goose Team during LudumDare 51. All rights reserved. "


#include "GameplayObjects/Goose.h"

#include "Components/AudioComponent.h"
#include "Components/TriggerComponents/InteractionSphereComponent.h"
#include "Core/Session/SessionGameMode.h"
#include "Particles/ParticleSystemComponent.h"

constexpr float HonkInterval = 10.f;

AGoose::AGoose()
{
	PrimaryActorTick.bCanEverTick = true;

	GooseRoot = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(GooseRoot);
	
	GooseMesh = CreateDefaultSubobject<UStaticMeshComponent>("GooseMesh");
	GooseMesh->SetupAttachment(GetRootComponent());

	AudioComponent = CreateDefaultSubobject<UAudioComponent>("HonkComponent");
	AudioComponent->SetupAttachment(GooseMesh);
	AudioComponent->SetAutoActivate(false);

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>("HonkParticle");
	ParticleSystemComponent->SetupAttachment(GetRootComponent());
	ParticleSystemComponent->SetAutoActivate(false);

	InteractionTrigger = CreateDefaultSubobject<UInteractionSphereComponent>("InteractionTrigger");
	InteractionTrigger->SetupAttachment(GetRootComponent());
	InteractionTrigger->InteractionMessage = FString{"Pet the Goose!"};
	InteractionTrigger->bRequireLineOfSight = true;
}

void AGoose::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetWorld())
	{
		ASessionGameMode* SessionGameMode = Cast<ASessionGameMode>(GetWorld()->GetAuthGameMode());

		if (SessionGameMode)
		{
			SessionGameMode->OnSessionStateChanged.AddDynamic(this, &AGoose::OnSessionStateChange);
		}
	}
}

void AGoose::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AnimatePosition();
	AnimateRotation();
}

bool AGoose::ProcessInteraction_Implementation(AActor* TargetActor)
{
	SetHonkingEnabled(false);
	
	if (GetWorld())
	{
		ASessionGameMode* SessionGameMode = Cast<ASessionGameMode>(GetWorld()->GetAuthGameMode());

		if (SessionGameMode)
		{
			SessionGameMode->FinishSession(true);
		}
	}
	
	return IInteractionInterface::ProcessInteraction_Implementation(TargetActor);
}

void AGoose::OnSessionStateChange(const ESessionState NewState)
{
	if (NewState != ESessionState::Progress) return;

	SetHonkingEnabled(true);
}

void AGoose::SetHonkingEnabled(const bool bIsEnabled)
{
	if (bIsEnabled)
	{
		GetWorld()->GetTimerManager().SetTimer(HonkingTimerHandle, this, &AGoose::PlayHonk, HonkInterval, true);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(HonkingTimerHandle);
	}
}

void AGoose::PlayHonk()
{
	AudioComponent->Play();
	ParticleSystemComponent->Activate();

	if (GooseMeshes.Num() == 0) return;
	
	GooseMesh->SetStaticMesh(GooseMeshes[FMath::RandRange(0, GooseMeshes.Num()-1)]);
}

void AGoose::AnimateRotation() const
{
	if (!bAnimateRotation) return;

	GooseMesh->AddRelativeRotation(FRotator(0.f, RotationSpeed, 0.f));
}

void AGoose::AnimatePosition() const
{
	if (!bAnimatePosition || !GetWorld()) return;

	const float Time = GetWorld()->GetTimeSeconds();
	FVector CurrentLocation = GooseMesh->GetRelativeLocation();
	CurrentLocation.Z = InitialLocation.Z + Amplitude * FMath::Sin(Frequency * Time);
	GooseMesh->SetRelativeLocation(CurrentLocation);
}

