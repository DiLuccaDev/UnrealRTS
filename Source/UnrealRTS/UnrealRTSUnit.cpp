// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealRTSUnit.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"

AUnrealRTSUnit::AUnrealRTSUnit()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	if (GetMesh())
	{
		GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AUnrealRTSUnit::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CachedDestination != FVector::ZeroVector)
	{
		UE_LOG(LogTemp, Display, TEXT("NONZERO VEC: %s"), *CachedDestination.ToString());

		if (FVector::DistSquared(GetActorLocation(), CachedDestination) > 100.f)
		{
			const FVector WorldDirection = (CachedDestination - GetActorLocation()).GetSafeNormal();
			AddMovementInput(WorldDirection, 1.f, false);
		} else
		{
			CachedDestination = FVector::ZeroVector;
		}
	}
}

void AUnrealRTSUnit::SetDestination(FVector Destination)
{
	CachedDestination = Destination;
	//MoveToDestination();
}

void AUnrealRTSUnit::MoveToDestination()
{
	if (CachedDestination == FVector::ZeroVector) return; // No Destination Set

	AsyncTask(ENamedThreads::GameThread, [this]()
	{
		while (FVector::DistSquared(GetActorLocation(), CachedDestination) > 100.f)
		{
			const FVector WorldDirection = (CachedDestination - GetActorLocation()).GetSafeNormal();
			AddMovementInput(WorldDirection, 1.f, false);

			//FPlatformProcess::Sleep(0.016f);
		}

		CachedDestination = FVector::ZeroVector;
	});
}
