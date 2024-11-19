// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameModeBase.generated.h"

/**
 * 
 */

class UFPSSaveGame;

UCLASS()
class FPSSHOTING_API AFPSGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	

public:

	void SaveGame(UFPSSaveGame* SaveGame);


	UFPSSaveGame* GetSaveData();
};
