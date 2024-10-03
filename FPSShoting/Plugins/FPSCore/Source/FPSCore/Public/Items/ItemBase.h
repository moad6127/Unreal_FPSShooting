// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionBase.h"
#include "ItemBase.generated.h"

/**
 * 
 */
UCLASS()
class FPSCORE_API AItemBase : public AInteractionBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void UpdateInteractionPopupText(const FText NewInteractionText) { InteractionText = NewInteractionText; }

	virtual void Interact() override;
};
