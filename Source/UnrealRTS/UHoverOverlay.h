// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Components/Image.h"
#include "UObject/ObjectMacros.h"
#include "Components/Overlay.h"

#include "UHoverOverlay.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMouseHoverEvent);

/**
 * 
 */
UCLASS()
class UNREALRTS_API UHoverOverlay : public UOverlay
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintCallable, Category = "HoverOverlay|Event")
	FOnMouseHoverEvent OnHovered;

	UPROPERTY(BlueprintCallable, Category = "HoverOverlay|Event")
	FOnMouseHoverEvent OnUnhovered;

protected:
	// References for editor-made widgets.
	UPROPERTY(EditAnywhere, meta = (BindWidget), Category = "HoverOverlay|Zones")
	UImage* TopZone;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget), Category = "HoverOverlay|Zones")
	UImage* LeftZone;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget), Category = "HoverOverlay|Zones")
	UImage* RightZone;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget), Category = "HoverOverlay|Zones")
	UImage* BottomZone;
};
