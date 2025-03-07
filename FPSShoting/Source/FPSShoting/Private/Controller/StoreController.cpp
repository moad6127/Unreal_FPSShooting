// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/StoreController.h"
#include "Widget/BasicWidget.h"
#include "Widget/BasicWidgetController.h"
#include "Components/EquipInventoryComponent.h"
#include "Items/ItemObject.h"
#include "GameMode/FPSGameModeBase.h"
#include "GameMode/FPSSaveGame.h"
#include "Kismet/GameplayStatics.h"

AStoreController::AStoreController()
{
	ContainerInventoryComponent = CreateDefaultSubobject<UEquipInventoryComponent>("ContainerEquipInventory");
}

void AStoreController::SaveItems()
{
	AFPSGameModeBase* GameMode = Cast<AFPSGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		UFPSSaveGame* SaveData = GameMode->GetSaveData();
		if (SaveData == nullptr)
		{
			return;
		}
		SaveContainerItems(SaveData);

		GameMode->SaveGame(SaveData);
	}
}

void AStoreController::SaveContainerItems(UFPSSaveGame* SaveData)
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

void AStoreController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	LoadData();
}

void AStoreController::BeginPlay()
{
	Super::BeginPlay();
	FWidgetControllerParams ContainerControllerParams(this, ContainerInventoryComponent);

	if (StoreWidgetControllerClass)
	{
		StoreWidgetController = NewObject<UBasicWidgetController>(this, StoreWidgetControllerClass);
		if (StoreWidgetController)
		{
			StoreWidgetController->SetWidgetControllerParams(ContainerControllerParams);
			if (StoreWidgetClass)
			{
				StoreWidget = CreateWidget<UBasicWidget>(this, StoreWidgetClass);
				if (StoreWidget)
				{
					StoreWidget->SetWidgetController(StoreWidgetController);
					StoreWidget->AddToViewport();
					bShowMouseCursor = true;
				}
			}
		}
	}
}

void AStoreController::LoadData()
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
			// â�� ������
			for (FItemSaveData Data : SaveData->ContainerItems)
			{
				UItemObject* ItemObject = GameMode->GetSaveItemFromItemData(Data);

				ContainerInventoryComponent->AddLoadedInventoryItem(ItemObject);
			}
		}
	}
}
