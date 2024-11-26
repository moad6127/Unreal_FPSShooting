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

UCLASS()
class FPSSHOTING_API AFPSGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	

public:

	void SaveGame(UFPSSaveGame* SaveGame);
	void DeleteSlot(UFPSSaveGame* SaveGame);

	UFPSSaveGame* GetSaveData();



	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> FPSSaveGameClass;

	UPROPERTY(EditAnywhere, Category = "Data Table")
	TObjectPtr<UDataTable> ItemDataTable;
};
