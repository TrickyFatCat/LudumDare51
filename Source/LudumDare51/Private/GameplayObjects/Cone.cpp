// "Copyright (c) 2022. Made by Title Goose Team during LudumDare 51. All rights reserved. "


#include "GameplayObjects/Cone.h"

#include "Components/HealthComponent.h"
#include "Components/StunComponent.h"
#include "Components/TriggerComponents/BaseCapsuleTriggerComponent.h"
#include "GameFramework/Character.h"


ACone::ACone()
{
	PrimaryActorTick.bCanEverTick = true;

	ConeRoot = CreateDefaultSubobject<USceneComponent>("ConeRoot");
	SetRootComponent(ConeRoot);

	ConeMesh = CreateDefaultSubobject<UStaticMeshComponent>("ConeMesh");
	ConeMesh->SetupAttachment(GetRootComponent());

	DamageTrigger = CreateDefaultSubobject<UBaseCapsuleTriggerComponent>("DamageTrigger");
	DamageTrigger->SetupAttachment(ConeMesh);
}

void ACone::BeginPlay()
{
	Super::BeginPlay();

	if (DamageTrigger)
	{
		DamageTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACone::OnTriggerBeginOverlap);
	}
}

void ACone::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                  const FHitResult& SweepResult)
{
	UHealthComponent* HealthComponent = OtherActor->FindComponentByClass<UHealthComponent>();

	if (HealthComponent)
	{
		HealthComponent->ApplyDamage(1);
	}

	UStunComponent* StunComponent = OtherActor->FindComponentByClass<UStunComponent>();

	if (StunComponent)
	{
		StunComponent->ApplyStun(StunDuration);
	}

	ACharacter* Character = Cast<ACharacter>(OtherActor);

	if (Character)
	{
		const FVector LaunchDirection = (Character->GetActorLocation() - GetActorLocation()).GetUnsafeNormal().
			GetSignVector();
		Character->LaunchCharacter(LaunchDirection * LaunchPower, true, true);
	}
}

void ACone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
