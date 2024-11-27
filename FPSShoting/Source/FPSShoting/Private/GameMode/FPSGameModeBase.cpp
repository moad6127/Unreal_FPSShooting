// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/FPSGameModeBase.h"
#include "GameMode/FPSSaveGame.h"
#include "Kismet/GameplayStatics.h"

void AFPSGameModeBase::SaveGame(UFPSSaveGame* SaveGame)
{
	DeleteSlot(SaveGame);
	UGameplayStatics::SaveGameToSlot(SaveGame, SaveGame->SlotName, SaveGame->SlotIndex);
}

void AFPSGameModeBase::DeleteSlot(UFPSSaveGame* SaveGame)
{
	if (UGameplayStatics::DoesSaveGameExist(SaveGame->SlotName, SaveGame->SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(SaveGame->SlotName, SaveGame->SlotIndex);
	}
}

UFPSSaveGame* AFPSGameModeBase::GetSaveData()
{
	USaveGame* SaveGameObject{};
	if (UGameplayStatics::DoesSaveGameExist(FString("SaveGame"), 0))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(FString("SaveGame"), 0);
	}
	else
	{
		if (FPSSaveGameClass)
		{
			SaveGameObject = UGameplayStatics::CreateSaveGameObject(FPSSaveGameClass);
		}
	}
	UFPSSaveGame* FPSSaveGame = Cast<UFPSSaveGame>(SaveGameObject);

	return FPSSaveGame;
}
