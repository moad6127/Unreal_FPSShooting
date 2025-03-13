// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/FPSGameModeBase.h"
#include "GameMode/FPSSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Items/ItemObject.h"

AFPSGameModeBase::AFPSGameModeBase()
{
	//bUseSeamlessTravel = true;
}

UItemObject* AFPSGameModeBase::GetSaveItemFromItemData(FItemSaveData Data)
{
	const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(Data.ItemID, Data.ItemID.ToString());
	if (!ItemData)
	{
		return nullptr;
	}
	UItemObject* ItemObject = NewObject<UItemObject>(this, UItemObject::StaticClass());
	//TODO : ItemValueSet
	ItemObject->ID = ItemData->ID;
	ItemObject->SlotType = ItemData->SlotType;
	ItemObject->ItemQuantity = Data.ItemQuantity;
	ItemObject->ItemNumbericData = ItemData->ItemNumbericData;
	ItemObject->Asset = ItemData->Asset;
	ItemObject->ItemName = ItemData->ItemName;
	ItemObject->WeaponData = ItemData->WeaponData;
	ItemObject->SetItemSizeX(ItemData->SizeX);
	ItemObject->SetItemSizeY(ItemData->SizeY);
	ItemObject->DataStruct = Data.DataStruct;
	ItemObject->SetItemItemLocation(Data.ItemLocation);
	ItemObject->SellCost = ItemData->SellCost;
	ItemObject->BuyCost = ItemData->BuyCost;

	return ItemObject;
}

UItemObject* AFPSGameModeBase::GetItemFromItemName(FName ItemName, int32 ItemQuantity)
{
	const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(ItemName, ItemName.ToString());
	if (!ItemData)
	{
		return nullptr;
	}
	UItemObject* ItemObject = NewObject<UItemObject>(this, UItemObject::StaticClass());
	//TODO : ItemValueSet
	ItemObject->ID = ItemData->ID;
	ItemObject->SlotType = ItemData->SlotType;
	ItemObject->ItemQuantity =ItemQuantity;
	ItemObject->ItemNumbericData = ItemData->ItemNumbericData;
	ItemObject->Asset = ItemData->Asset;
	ItemObject->ItemName = ItemData->ItemName;
	ItemObject->WeaponData = ItemData->WeaponData;
	ItemObject->SetItemSizeX(ItemData->SizeX);
	ItemObject->SetItemSizeY(ItemData->SizeY);
	ItemObject->SellCost = ItemData->SellCost;
	ItemObject->BuyCost = ItemData->BuyCost;

	ItemObject->InitDataStruct();

	return ItemObject;
}



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
