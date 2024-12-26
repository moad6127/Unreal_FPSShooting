// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "WeaponBase.h"
#include "FPSSaveGame.generated.h"

/**
 * 
 */

class UItemObject;
struct FRuntimeWeaponData;

USTRUCT()
struct FItemSaveData {

	GENERATED_BODY()

	UPROPERTY()
	FName ItemID = FName();

	UPROPERTY()
	int32 ItemQuantity = 0;

	UPROPERTY()
	FIntPoint ItemLocation = FIntPoint();

	UPROPERTY()
	FRuntimeWeaponData DataStruct = FRuntimeWeaponData();

	UPROPERTY()
	bool bEquipped = false;
};

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
	TArray<FItemSaveData> InventoryItems;
};
