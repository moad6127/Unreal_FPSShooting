// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/ContainerController.h"
#include "Widget/BasicWidget.h"
#include "Widget/BasicWidgetController.h"
#include "Components/EquipInventoryComponent.h"
#include "Items/ItemObject.h"
#include "GameMode/FPSGameModeBase.h"
#include "GameMode/FPSSaveGame.h"
#include "Kismet/GameplayStatics.h"

AContainerController::AContainerController()
{
	ContainerInventoryComponent = CreateDefaultSubobject<UEquipInventoryComponent>("ContainerEquipInventory");
	PlayerInventoryComponent = CreateDefaultSubobject<UEquipInventoryComponent>("PlayerEquipInventory");
}

void AContainerController::SaveItems()
{
	AFPSGameModeBase* GameMode = Cast<AFPSGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		UFPSSaveGame* SaveData = GameMode->GetSaveData();
		if (SaveData == nullptr)
		{
			return;
		}
		SaveCharacterItems(SaveData);
		SaveCharacterEquips(SaveData);
		SaveContainerItems(SaveData);

		GameMode->SaveGame(SaveData);
	}
}

void AContainerController::SaveCharacterItems(UFPSSaveGame* SaveData)
{
	SaveData->InventoryItems.Empty();
	//TODO : ItemObject�� Copy�ؼ� ���� �κ��丮�� �̻��ϰ� ����,
	// ����ü�� ���� �������� ������ ������ ����ؾ� �ҰͰ���.

	// Inventory������ �����ϱ�
	for (UItemObject* Item : PlayerInventoryComponent->GetInventoryItems())
	{
		FItemSaveData ItemData;
		ItemData.ItemID = Item->ID;
		ItemData.ItemQuantity = Item->ItemQuantity;
		ItemData.DataStruct = Item->DataStruct;
		ItemData.ItemLocation = Item->GetItemItemLocation();
		ItemData.bEquipped = false;

		SaveData->InventoryItems.Add(ItemData);
	}
}

void AContainerController::SaveCharacterEquips(UFPSSaveGame* SaveData)
{
	FEquipmentItems CharacterEquipmentItems = PlayerInventoryComponent->GetEquipmentItems();

	if (CharacterEquipmentItems.Head)
	{
		FItemSaveData HeadItemData;
		HeadItemData.bEquipped = true;
		HeadItemData.ItemID = CharacterEquipmentItems.Head->ID;
		SaveData->InventoryItems.Add(HeadItemData);
	}
	if (CharacterEquipmentItems.Body)
	{
		FItemSaveData BodyItemData;
		BodyItemData.bEquipped = true;
		BodyItemData.ItemID = CharacterEquipmentItems.Body->ID;
		SaveData->InventoryItems.Add(BodyItemData);
	}
	if (CharacterEquipmentItems.BackPack)
	{
		FItemSaveData BackPackItemData;
		BackPackItemData.bEquipped = true;
		BackPackItemData.ItemID = CharacterEquipmentItems.BackPack->ID;
		SaveData->InventoryItems.Add(BackPackItemData);
	}
	if (CharacterEquipmentItems.Weapon1)
	{
		FItemSaveData Weapon1ItemData;
		Weapon1ItemData.bEquipped = true;
		Weapon1ItemData.ItemID = CharacterEquipmentItems.Weapon1->ID;
		Weapon1ItemData.DataStruct = CharacterEquipmentItems.Weapon1->DataStruct;
		SaveData->InventoryItems.Add(Weapon1ItemData);
	}
	if (CharacterEquipmentItems.Weapon2)
	{
		FItemSaveData Weapon2ItemData;
		Weapon2ItemData.bEquipped = true;
		Weapon2ItemData.ItemID = CharacterEquipmentItems.Weapon2->ID;
		Weapon2ItemData.DataStruct = CharacterEquipmentItems.Weapon2->DataStruct;
		SaveData->InventoryItems.Add(Weapon2ItemData);
	}
}

void AContainerController::SaveContainerItems(UFPSSaveGame* SaveData)
{
	SaveData->ContainerItems.Empty();
	//TODO : ItemObject�� Copy�ؼ� ���� �κ��丮�� �̻��ϰ� ����,
	// ����ü�� ���� �������� ������ ������ ����ؾ� �ҰͰ���.

	// Inventory������ �����ϱ�
	for (UItemObject* Item : ContainerInventoryComponent->GetInventoryItems())
	{
		FItemSaveData ItemData;
		ItemData.ItemID = Item->ID;
		ItemData.ItemQuantity = Item->ItemQuantity;
		ItemData.DataStruct = Item->DataStruct;
		ItemData.ItemLocation = Item->GetItemItemLocation();
		ItemData.bEquipped = false;

		SaveData->ContainerItems.Add(ItemData);
	}
}

void AContainerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	LoadData();
}

void AContainerController::BeginPlay()
{
	Super::BeginPlay();
	FWidgetControllerParams PlayerControllerParams(this, PlayerInventoryComponent);
	FWidgetControllerParams ContainerControllerParams(this, ContainerInventoryComponent);

	if (ContainerWidgetControllerClass)
	{
		ContainerWidgetController = NewObject<UBasicWidgetController>(this, ContainerWidgetControllerClass);
		PlayerInventoryWidgetController = NewObject<UBasicWidgetController>(this, ContainerWidgetControllerClass);
		if (ContainerWidgetController)
		{
			ContainerWidgetController->SetWidgetControllerParams(ContainerControllerParams);
			PlayerInventoryWidgetController->SetWidgetControllerParams(PlayerControllerParams);
			if (ContainerWidgetClass)
			{
				ContainerWidget = CreateWidget<UBasicWidget>(this, ContainerWidgetClass);
				if (ContainerWidget)
				{
					ContainerWidget->SetWidgetController(ContainerWidgetController);
					ContainerWidget->AddToViewport();
					bShowMouseCursor = true;
				}
			}
		}
	}
}

void AContainerController::LoadData()
{

	AFPSGameModeBase* GameMode = Cast<AFPSGameModeBase>(UGameplayStatics::GetGameMode(this));
	{
		if (GameMode)
		{
			UFPSSaveGame* SaveData = GameMode->GetSaveData();
			if (SaveData == nullptr)
			{
				return;
			}
			UDataTable* ItemDataTable = GameMode->ItemDataTable;
			if (!ItemDataTable)
			{
				return;
			}
			//�κ��丮, ���â ������
			for (FItemSaveData Data : SaveData->InventoryItems)
			{
				UItemObject* ItemObject = GameMode->GetSaveItemFromItemData(Data);

				//������ ���¿����� �����ϵ��� �����
				if (Data.bEquipped)
				{
					PlayerInventoryComponent->AddLoadedEquipItem(ItemObject);
				}
				else
				{
					PlayerInventoryComponent->AddLoadedInventoryItem(ItemObject);
				}
			}
			// â�� ������
			for (FItemSaveData Data : SaveData->ContainerItems)
			{
				UItemObject* ItemObject = GameMode->GetSaveItemFromItemData(Data);

				ContainerInventoryComponent->AddLoadedInventoryItem(ItemObject);
			}
		}
	}
}
