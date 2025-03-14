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
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"

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
			//TODO : Character의 인벤, 장비창 로드하기
			for (FItemSaveData Data : SaveData->InventoryItems)
			{
				UItemObject* ItemObject = GameMode->GetSaveItemFromItemData(Data);

				//장착된 상태였으면 장착하도록 만들기
				if (Data.bEquipped)
				{
					GetEquipInventoryComponent()->AddLoadedEquipItem(ItemObject);
				}
				else
				{
					GetEquipInventoryComponent()->AddLoadedInventoryItem(ItemObject);
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
		Weapon1ItemData.DataStruct = *GetInventoryComponent()->GetWeaponByID(0)->GetRuntimeWeaponData();
		SaveData->InventoryItems.Add(Weapon1ItemData);
	}
	if (CharacterEquipmentItems.Weapon2)
	{
		FItemSaveData Weapon2ItemData;
		Weapon2ItemData.bEquipped = true;
		Weapon2ItemData.ItemID = CharacterEquipmentItems.Weapon2->ID;
		Weapon2ItemData.DataStruct = *GetInventoryComponent()->GetWeaponByID(1)->GetRuntimeWeaponData();
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
		//TODO : Character의 인벤, 장비창 세이브 하기
		SaveData->InventoryItems.Empty();
		//TODO : ItemObject를 Copy해서 만들어도 인벤토리에 이상하게 들어간다,
		// 구조체를 만들어서 아이템의 정보를 저장해 사용해야 할것같다.

		// Inventory아이템 저장하기
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

		//TODO : 장착된 아이템 저장하기
		EquipItemSave(SaveData);

		//탈출 보너스 코인들
		SaveData->Coins += 1000;

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
		if (InventoryAction)
		{
			PlayerEnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Completed, this, &ASInvenFPSCharacter::ShowInventory);

		}
		if (PauseAction)
		{
			PlayerEnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Completed, this, &ASInvenFPSCharacter::GamePause);
		}
	}
}

void ASInvenFPSCharacter::ShowInventory()
{
	if (HUD)
	{
		HUD->ShowInventory();
	}
}

void ASInvenFPSCharacter::GamePause()
{
	if (HUD)
	{
		HUD->ShowGamePause();
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
