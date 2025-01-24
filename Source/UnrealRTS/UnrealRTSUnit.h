// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UnrealRTSUnit.generated.h"

UCLASS(Blueprintable)
class AUnrealRTSUnit : public ACharacter
{
	GENERATED_BODY()

public:
	AUnrealRTSUnit();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;
};

