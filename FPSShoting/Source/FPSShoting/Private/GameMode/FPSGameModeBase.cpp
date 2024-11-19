// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/FPSGameModeBase.h"
#include "GameMode/FPSSaveGame.h"
#include "Kismet/GameplayStatics.h"

void AFPSGameModeBase::SaveGame(UFPSSaveGame* SaveGame)
{
	UGameplayStatics::SaveGameToSlot(SaveGame, SaveGame->SlotName, SaveGame->SlotIndex);
}

UFPSSaveGame* AFPSGameModeBase::GetSaveData()
{
	USaveGame* SaveGameObject = nullptr;
	if (UGameplayStatics::DoesSaveGameExist("SaveGame", 0))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot("SaveGame", 0);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(USaveGame::StaticClass());
	}
	UFPSSaveGame* FPSSaveGame = Cast<UFPSSaveGame>(SaveGameObject);
	return FPSSaveGame;
}
