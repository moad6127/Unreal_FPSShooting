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

void AContainerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	LoadData();
}

void AContainerController::BeginPlay()
{
	Super::BeginPlay();
	FWidgetControllerParams ControllerParams(this, ContainerInventoryComponent);
	FWidgetControllerParams PlayerControllerParams(this, PlayerInventoryComponent);

	if (ContainerWidgetControllerClass)
	{
		ContainerWidgetController = NewObject<UBasicWidgetController>(this, ContainerWidgetControllerClass);
		PlayerInventoryWidgetController = NewObject<UBasicWidgetController>(this, ContainerWidgetControllerClass);
		if (ContainerWidgetController)
		{
			ContainerWidgetController->SetWidgetControllerParams(ControllerParams);
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
			//인벤토리, 장비창 아이템
			for (FItemSaveData Data : SaveData->InventoryItems)
			{
				const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(Data.ItemID, Data.ItemID.ToString());
				if (!ItemData)
				{
					return;
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

				//장착된 상태였으면 장착하도록 만들기
				if (Data.bEquipped)
				{
					PlayerInventoryComponent->AddLoadedEquipItem(ItemObject);
				}
				else
				{
					PlayerInventoryComponent->AddLoadedInventoryItem(ItemObject);
				}
			}
			// 창고 아이템
			for (FItemSaveData Data : SaveData->ContainerItems)
			{
				const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(Data.ItemID, Data.ItemID.ToString());
				if (!ItemData)
				{
					return;
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

				ContainerInventoryComponent->AddLoadedInventoryItem(ItemObject);
			}
		}
	}
}
