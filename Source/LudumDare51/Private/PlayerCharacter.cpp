// Copyright (c) 2022. Made by Title Goose Team during LudumDare 51. All rights reserved. 


#include "PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/DashComponent.h"
#include "Components/HealthComponent.h"
#include "Components/InteractionQueueComponent.h"
#include "Components/KeyRingComponent.h"
#include "Core/Session/SessionGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera Component");
	CameraComponent->SetupAttachment(GetMesh());
	
	InteractionQueueComponent = CreateDefaultSubobject<UInteractionQueueComponent>("InteractionComponent");

	KeyRingComponent = CreateDefaultSubobject<UKeyRingComponent>("KeyRing");

	DashComponent = CreateDefaultSubobject<UDashComponent>("DashComponent");

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	CameraInitialLocation = CameraComponent->GetRelativeLocation();
	HealthComponent->OnDeath.AddDynamic(this, &APlayerCharacter::OnDeath);
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	if (!GetCharacterMovement()->IsFalling())
	{
		float NewZ = 0.f;

		if (GetCharacterMovement()->IsCrouching())
		{
			NewZ = FMath::Lerp(CameraComponent->GetRelativeLocation().Z, CrouchedEyeHeight, CameraTransitionSpeed);
		}
		else
		{
			NewZ = FMath::Lerp(CameraComponent->GetRelativeLocation().Z, BaseEyeHeight, CameraTransitionSpeed);
		}

		const FVector NewLocation = FVector(CameraInitialLocation.X, CameraInitialLocation.Y, NewZ);
		CameraComponent->SetRelativeLocation(NewLocation);
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Movement	
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::Jump);
	
	DECLARE_DELEGATE_TwoParams(FCrouchDelegate, const bool, const bool);
	PlayerInputComponent->BindAction<FCrouchDelegate>("Crouch", IE_Pressed, this, &APlayerCharacter::ToggleCrouch, true, false);
	PlayerInputComponent->BindAction<FCrouchDelegate>("Crouch", IE_Released, this, &APlayerCharacter::ToggleCrouch, false, false);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &APlayerCharacter::Dash);

	//Aim
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookRight", this, &APlayerCharacter::AddControllerYawInput);

	//Other
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::Interact);
}

void APlayerCharacter::MoveForward(float AxisValue)
{
	LateralDirection.X = AxisValue;
	AddMovementInput(GetActorForwardVector(), AxisValue);
}

void APlayerCharacter::MoveRight(float AxisValue)
{
	LateralDirection.Y = AxisValue;
	AddMovementInput(GetActorRightVector(), AxisValue);
}

void APlayerCharacter::Interact()
{
	InteractionQueueComponent->Interact();
}

void APlayerCharacter::Dash()
{
	FVector Direction = GetActorForwardVector();

	if (LateralDirection != FVector::ZeroVector)
	{
		Direction.X = GetVelocity().GetSafeNormal().X;
		Direction.Y = GetVelocity().GetSafeNormal().Y;
		Direction.Z = 0.f;
	}
	
	DashComponent->Dash(Direction);
}

void APlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (bWantCrouch)
	{
		ToggleCrouch(true, true);
		bWantCrouch = false;
	}
}

void APlayerCharacter::ToggleCrouch(const bool bIsCrouching, const bool bForceCrouch)
{
	if (GetMovementComponent()->IsFalling() && !bForceCrouch)
	{
		bWantCrouch = true;
		return;
	}

	bIsCrouching ? Crouch(false) : UnCrouch(true);
}

void APlayerCharacter::OnDeath()
{
	if (GetWorld())
	{
		ASessionGameMode* SessionGameMode = Cast<ASessionGameMode>(GetWorld()->GetAuthGameMode());

		if (SessionGameMode)
		{
			SessionGameMode->FinishSession(false);
		}
	}
}

