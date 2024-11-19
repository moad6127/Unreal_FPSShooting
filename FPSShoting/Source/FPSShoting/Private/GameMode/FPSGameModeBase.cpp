// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/FPSGameModeBase.h"
#include "GameMode/FPSSaveGame.h"
#include "Kismet/GameplayStatics.h"

void AFPSGameModeBase::SaveGame()
{
	UFPSSaveGame* SaveGameInstance = Cast<UFPSSaveGame>(UGameplayStatics::CreateSaveGameObject(UFPSSaveGame::StaticClass()));

	if (SaveGameInstance)
	{

	}
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SlotName, SaveGameInstance->SlotIndex);
}

void AFPSGameModeBase::LoadGame()
{
	UFPSSaveGame* LoadGameInstance = Cast<UFPSSaveGame>(UGameplayStatics::CreateSaveGameObject(UFPSSaveGame::StaticClass()));
	
	if (LoadGameInstance)
	{
		LoadGameInstance = Cast<UFPSSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SlotName, LoadGameInstance->SlotIndex));

	}

}
