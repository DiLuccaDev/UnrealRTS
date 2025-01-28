// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerHUDWidgetCode.h"
#include "GameFramework/HUD.h"
#include "PlayerHUDCode.generated.h"

/**
 * 
 */
UCLASS()
class UNREALRTS_API APlayerHUDCode : public AHUD
{
	GENERATED_BODY()

public:
	APlayerHUDCode();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPlayerHUDWidgetCode* HUDWidget;

protected:
	virtual void BeginPlay() override;
};
