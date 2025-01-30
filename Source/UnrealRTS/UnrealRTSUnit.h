// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Selectable.h"
#include "GameFramework/Character.h"
#include "UnrealRTSUnit.generated.h"

UCLASS(Blueprintable)
class AUnrealRTSUnit : public ACharacter, public ISelectable
{
	GENERATED_BODY()

public:
	AUnrealRTSUnit();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetDestination(FVector Destination);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
	FString UnitName = FString("UNIT NAME NOT SET");

protected:

private:
	FVector CachedDestination;

	UFUNCTION()
	void MoveToDestination();
};

