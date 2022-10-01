// Copyright (c) 2022. Made by Title Goose Team during LudumDare 51. All rights reserved. 


#include "GameplayObjects/JumpPad.h"

#include "PlayerCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/TriggerComponents/BaseSphereTriggerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/SceneComponent.h"

AJumpPad::AJumpPad()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	JumpTrigger = CreateDefaultSubobject<UBaseSphereTriggerComponent>("JumpTrigger");
	JumpTrigger->SetupAttachment(GetRootComponent());

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(GetRootComponent());

	JumpDirection = CreateDefaultSubobject<UArrowComponent>("JumpDirection");
	JumpDirection->SetupAttachment(GetRootComponent());
	JumpDirection->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
}

void AJumpPad::BeginPlay()
{
	Super::BeginPlay();

	JumpTrigger->OnComponentBeginOverlap.AddDynamic(this, &AJumpPad::OnTriggerBeginOverlap);
}

void AJumpPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AJumpPad::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
									 AActor* OtherActor,
									 UPrimitiveComponent* OtherComp,
									 int32 OtherBodyIndex,
									 bool bFromSweep,
									 const FHitResult& SweepResult)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);

	if (!PlayerCharacter) return;

	PlayerCharacter->LaunchCharacter(JumpDirection->GetComponentRotation().Vector() * JumpForce, true, true);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), JumpSound, GetActorLocation(), GetActorRotation());
}
