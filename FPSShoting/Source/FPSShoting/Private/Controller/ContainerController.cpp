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
			//인벤토리, 장비창 아이템
			for (FItemSaveData Data : SaveData->InventoryItems)
			{
				UItemObject* ItemObject = GameMode->GetSaveItemFromItemData(Data);

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
				UItemObject* ItemObject = GameMode->GetSaveItemFromItemData(Data);

				ContainerInventoryComponent->AddLoadedInventoryItem(ItemObject);
			}
		}
	}
}
