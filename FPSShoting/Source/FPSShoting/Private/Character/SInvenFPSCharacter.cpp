// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SInvenFPSCharacter.h"
#include "GameMode/FPSGameModeBase.h"
#include "GameMode/FPSSaveGame.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InventoryComponent.h"
#include "HUD/FPSHUD.h"
#include "WeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "FPSCharacterController.h"
#include "Components/EquipInventoryComponent.h"
#include "Items/ItemObject.h"
#include "Items/ItemBase.h"


ASInvenFPSCharacter::ASInvenFPSCharacter()
{
}

void ASInvenFPSCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	LoadGame();
}

void ASInvenFPSCharacter::LoadGame()
{
	if (!IsLocallyControlled())
	{
		return;
	}

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
			//TODO : Character�� �κ�, ���â �ε��ϱ�
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

				//������ ���¿����� �����ϵ��� �����
				if (Data.bEquipped)
				{
					GetEquipInventoryComponent()->EquipItem(ItemObject);
				}
				else
				{
					if (!GetEquipInventoryComponent()->AddItem(ItemObject))
					{
						UE_LOG(LogTemp, Warning, TEXT("LoadFail!!!!"));
					}
				}
			}
			UE_LOG(LogTemp, Warning, TEXT("PlayerLoadGameFunc!"));

		}
	}
}

void ASInvenFPSCharacter::EquipItemSave(UFPSSaveGame* SaveData)
{
	FEquipmentItems CharacterEquipmentItems = GetEquipInventoryComponent()->GetEquipmentItems();

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

void ASInvenFPSCharacter::SaveGame()
{
	AFPSGameModeBase* GameMode = Cast<AFPSGameModeBase>(UGameplayStatics::GetGameMode(this));

	if (GameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("InGameMode!"));
		UFPSSaveGame* SaveData = GameMode->GetSaveData();
		UE_LOG(LogTemp, Warning, TEXT("SaveDataGet!"));
		if (SaveData == nullptr)
		{
			return;
		}
		//TODO : Character�� �κ�, ���â ���̺� �ϱ�
		SaveData->InventoryItems.Empty();
		//TODO : ItemObject�� Copy�ؼ� ���� �κ��丮�� �̻��ϰ� ����,
		// ����ü�� ���� �������� ������ ������ ����ؾ� �ҰͰ���.

		// Inventory������ �����ϱ�
		for (UItemObject* Item : GetEquipInventoryComponent()->GetInventoryItems())
		{
			FItemSaveData ItemData;
			ItemData.ItemID = Item->ID;
			ItemData.ItemQuantity = Item->ItemQuantity;
			ItemData.DataStruct = Item->DataStruct;
			ItemData.ItemLocation = Item->GetItemItemLocation();
			ItemData.bEquipped = false;

			SaveData->InventoryItems.Add(ItemData);
		}

		//TODO : ������ ������ �����ϱ�
		EquipItemSave(SaveData);

		UE_LOG(LogTemp, Warning, TEXT("PlayerSaveGameFunc!"));
		GameMode->SaveGame(SaveData);
	}
}

AFPSHUD* ASInvenFPSCharacter::GetHUD() const
{
	if (HUD)
	{
		return HUD;
	}
	return nullptr;
}

void ASInvenFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		HUD = Cast<AFPSHUD>(PlayerController->GetHUD());
		if (HUD)
		{
			HUD->InitHUD(PlayerController, SInventoryComponent);
		}
	}
}

void ASInvenFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		PlayerEnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Completed, this, &ASInvenFPSCharacter::ShowInventory);
	}
}

void ASInvenFPSCharacter::ShowInventory()
{
	if (HUD)
	{
		HUD->ShowInventory();
	}
}



void ASInvenFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetHUDCrosshair(DeltaTime);
}


void ASInvenFPSCharacter::SetHUDCrosshair(float DeltaTime)
{
	if (HUD)
	{
		FHUDPackage HUDPackage;
		HUDPackage.bDrawCrosshair = bShowCrosshair;
		if (GetInventoryComponent())
		{
			if (GetInventoryComponent()->GetCurrentWeapon())
			{
				FStaticWeaponData WeaponData = *GetInventoryComponent()->GetCurrentWeapon()->GetStaticWeaponData();
				HUDPackage.CrosshairCenter = WeaponData.CrosshairCenter;
				HUDPackage.CrosshairTop = WeaponData.CrosshairTop;
				HUDPackage.CrosshairBottom = WeaponData.CrosshairDown;
				HUDPackage.CrosshairRight = WeaponData.CrosshairRight;
				HUDPackage.CrosshairLeft = WeaponData.CrosshairLeft;
			}
			else
			{
				HUDPackage.CrosshairCenter = nullptr;
				HUDPackage.CrosshairTop = nullptr;
				HUDPackage.CrosshairBottom = nullptr;
				HUDPackage.CrosshairRight = nullptr;
				HUDPackage.CrosshairLeft = nullptr;
			}
		}
		HUD->SetHUDPackage(HUDPackage);
	}
	

}
