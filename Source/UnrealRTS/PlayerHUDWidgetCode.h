// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UHoverOverlay.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidgetCode.generated.h"

/**
 * 
 */
UCLASS()
class UNREALRTS_API UPlayerHUDWidgetCode : public UUserWidget
{
	GENERATED_BODY()

public:
	// virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerHUD")
	TObjectPtr<UHoverOverlay> HoverOverlay;
	
protected:
	// virtual bool Initialize() override;
};
