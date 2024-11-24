// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "FPSSaveGame.generated.h"

/**
 * 
 */

class UItemObject;

UCLASS()
class FPSSHOTING_API UFPSSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	FString SlotName = FString("SaveGame");

	UPROPERTY()
	int32 SlotIndex = 0;

	UPROPERTY()
	TArray<UItemObject*> InventoryItems;
};
