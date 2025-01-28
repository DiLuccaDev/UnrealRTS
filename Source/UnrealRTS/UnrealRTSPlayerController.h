// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "PlayerHUDCode.h"

#include "UnrealRTSPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS()
class AUnrealRTSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AUnrealRTSPlayerController();

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float cameraPanKeyboardSensitivity = .25f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float cameraPanMouseSensitivity = .25f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float cameraPanSpeed = 2.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float cameraZoomSpeed = 10.f;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UNiagaraSystem> FXCursor;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SetDestinationAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SelectAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> CameraPanAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> CameraZoomAction;

	// Store a reference to the HUD
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=HUD)
	TObjectPtr<APlayerHUDCode> PlayerHUD;

	UFUNCTION(BlueprintCallable, Category=HUD)
	void SetPlayerHUD(APlayerHUDCode* InPlayerHUD);

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	virtual void SetupInputComponent() override;

	// To add mapping context
	virtual void BeginPlay();

	// Handle camera panning
	void PanCamera(const FVector& value);

	/** Input handlers for SetDestination action. */
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();

	/** Input handlers for Camera actions. */
	void OnCameraPanTriggered(const FInputActionValue& Value);
	void OnCameraZoomTriggered(const FInputActionValue& Value);

	/* TODO */
	UFUNCTION()
	void HandleHovered();
	void PanCamera(const float x, const float y);

private:
	FVector cachedDestination;

	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed
};
