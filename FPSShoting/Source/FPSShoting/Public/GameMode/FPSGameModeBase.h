// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameModeBase.generated.h"

/**
 * 
 */

class UFPSSaveGame;
class USaveGame;
class UDataTable;
class UItemObject;
struct FItemSaveData;

UCLASS()
class FPSSHOTING_API AFPSGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	

public:
	AFPSGameModeBase();

	UItemObject* GetSaveItemFromItemData(FItemSaveData Data);
	UItemObject* GetItemFromItemName(FName ItemName, int32 ItemQuantity = 1);
	void SaveGame(UFPSSaveGame* SaveGame);
	void DeleteSlot(UFPSSaveGame* SaveGame);

	UFPSSaveGame* GetSaveData();



	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> FPSSaveGameClass;

	UPROPERTY(EditAnywhere, Category = "Data Table")
	TObjectPtr<UDataTable> ItemDataTable;
};
