// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealRTSGameMode.h"
#include "UnrealRTSPlayerController.h"
#include "UnrealRTSPlayer.h"
#include "UObject/ConstructorHelpers.h"

AUnrealRTSGameMode::AUnrealRTSGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AUnrealRTSPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_Player"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_PlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}