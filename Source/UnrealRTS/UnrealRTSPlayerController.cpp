// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealRTSPlayerController.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/World.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Components/InputComponent.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "GameFramework/GameUserSettings.h"
#include "GameFramework/SpringArmComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AUnrealRTSPlayerController::AUnrealRTSPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	cachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

void AUnrealRTSPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// SetInputMode();
}

void AUnrealRTSPlayerController::PanCamera(const FVector& value)
{
	UE_LOG(LogTemp, Display, TEXT("Panning camera with V2: X=%f, Y=%f"), value.X, value.Y);
}

void AUnrealRTSPlayerController::SetPlayerHUD(APlayerHUDCode* InPlayerHUD)
{
	PlayerHUD = InPlayerHUD;

	//PanCamera(x,y)
	if (!PlayerHUD)
	{
		UE_LOG(LogTemp, Display, TEXT("Set player hud failed 1"));
	}
	else if (!PlayerHUD->HUDWidget)
	{
		UE_LOG(LogTemp, Display, TEXT("Set player hud failed 2"));
	}
	else if (!PlayerHUD->HUDWidget->HoverOverlay)
	{
		UE_LOG(LogTemp, Display, TEXT("Set player hud failed 3"));
	}
	else
	{
		UHoverOverlay* HoverOverlay = PlayerHUD->HUDWidget->HoverOverlay;
		HoverOverlay->OnHovered.AddDynamic(this, &AUnrealRTSPlayerController::HandleHovered);
	}
}

void AUnrealRTSPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(SetDestinationAction, ETriggerEvent::Started, this,
		                                   &AUnrealRTSPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationAction, ETriggerEvent::Triggered, this,
		                                   &AUnrealRTSPlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationAction, ETriggerEvent::Completed, this,
		                                   &AUnrealRTSPlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationAction, ETriggerEvent::Canceled, this,
		                                   &AUnrealRTSPlayerController::OnSetDestinationReleased);

		// Setup kb mouse panning
		EnhancedInputComponent->BindAction(CameraPanAction, ETriggerEvent::Triggered, this,
		                                   &AUnrealRTSPlayerController::OnCameraPanTriggered);
		EnhancedInputComponent->BindAction(CameraZoomAction, ETriggerEvent::Triggered, this,
		                                   &AUnrealRTSPlayerController::OnCameraZoomTriggered);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error,
		       TEXT(
			       "'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
		       ), *GetNameSafe(this));
	}
}

void AUnrealRTSPlayerController::OnInputStarted()
{
	StopMovement();
}

// Triggered every frame when the input is held down
void AUnrealRTSPlayerController::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();

	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		cachedDestination = Hit.Location;
	}

	// Move towards mouse pointer or touch
	// TODO: Change this based on user selection.
	/*
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (cachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
	*/
}

void AUnrealRTSPlayerController::OnSetDestinationReleased()
{
	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		//UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, cachedDestination, FRotator::ZeroRotator,
		                                               FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}

// Keyboard input for camera pan
void AUnrealRTSPlayerController::OnCameraPanTriggered(const FInputActionValue& Value)
{
	const FVector2D inputValue = Value.Get<FVector2D>() * cameraPanKeyboardSensitivity;
	PanCamera(inputValue.X, inputValue.Y);
}

// Mouse input for camera pan
void AUnrealRTSPlayerController::HandleHovered()
{
	FVector2D outVec;
	if (GetMousePosition(outVec.X, outVec.Y))
	{
		FVector2D ViewportSize;
			GEngine->GameViewport->GetViewportSize(ViewportSize);
		const FVector2D ViewportCenter = FVector2D(ViewportSize.X / 2, ViewportSize.Y / 2);
		
		// Calculate the offset from the center.
		FVector2D OffsetFromCenter = outVec - ViewportCenter;
		
		// Normalize the offset.
		FVector2D NormalizedOffset = OffsetFromCenter.GetSafeNormal();

		// Apply a nonlinear scaling (e.g., power curve).
		// TODO: Parameterize the exponent for mouse screen pan.
		FVector2D ScaledOffset;
		ScaledOffset.X = FMath::Sign(NormalizedOffset.X) * FMath::Pow(FMath::Abs(NormalizedOffset.X), 1.5f);
		ScaledOffset.Y = FMath::Sign(NormalizedOffset.Y) * FMath::Pow(FMath::Abs(NormalizedOffset.Y), 1.5f);

		/* PRINT/DRAW DEBUG
		 * Does not follow a moving camera,
		 *	consider commenting the PanCamera function below for debug
		GEngine->AddOnScreenDebugMessage(
			-1, // Key (use -1 for no overwrite) 
			5.0f, // Display time in seconds  
			FColor::Green, // Text color  
			FString::Printf(TEXT("X: %f, Y: %f"), ScaledOffset.X, ScaledOffset.Y)
		);
		UE_LOG(LogTemp, Display, TEXT("X: %f, Y: %f"), ScaledOffset.X, ScaledOffset.Y);
		// Draw a debug point at the ViewportCenter to check its location.
		FVector WorldLocation, WorldDirection;
		if (DeprojectScreenPositionToWorld(ViewportCenter.X, ViewportCenter.Y, WorldLocation, WorldDirection))
		{
			DrawDebugPoint(GetWorld(), WorldLocation, 10.0f, FColor::Red, false, 1.0f); // Adjust size/lifespan as needed.
		}
		*/
		
		// Apply sensitivity scaling.
		ScaledOffset *= cameraPanMouseSensitivity;

		PanCamera(ScaledOffset.X, -ScaledOffset.Y);
	}
}

void AUnrealRTSPlayerController::PanCamera(const float x, const float y)
{
	APawn* controlledPawn = GetPawn();
	if (controlledPawn != nullptr)
	{
		// SWAP X and Y For Unreal Left-Handed COORD System.
		FVector direction(y, x, .0f);
		controlledPawn->AddMovementInput(direction, cameraPanSpeed, true);
	}
}

void AUnrealRTSPlayerController::OnCameraZoomTriggered(const FInputActionValue& Value)
{
	APawn* controlledPawn = GetPawn();
	if (controlledPawn != nullptr)
	{
		const float inputValue = Value.Get<float>();
		USpringArmComponent* SpringArm = Cast<USpringArmComponent>(
			controlledPawn->GetComponentByClass(USpringArmComponent::StaticClass()));
		if (SpringArm)
		{
			GEngine->AddOnScreenDebugMessage(
				-1, // Key (use -1 for no overwrite)  
				.1f, // Display time in seconds  
				FColor::Green, // Text color  
				FString::Printf(TEXT("Value: %f"), SpringArm->TargetArmLength)
			);
			if (inputValue > 0 && SpringArm->TargetArmLength <= 50)
			{
				SpringArm->TargetArmLength = 50;
				return;
			}
			if (inputValue < 0 && SpringArm->TargetArmLength >= 3000)
			{
				SpringArm->TargetArmLength = 3000;
				return;
			}
			SpringArm->TargetArmLength -= (inputValue * cameraZoomSpeed);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("SpringArm component not found!"));
		}
	}
}
