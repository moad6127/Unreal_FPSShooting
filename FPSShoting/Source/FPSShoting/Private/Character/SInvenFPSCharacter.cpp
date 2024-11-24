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
	if (!Cast<APlayerController>(GetController()))
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
			//TODO : Character의 인벤, 장비창 로드하기
			UE_LOG(LogTemp, Warning, TEXT("PlayerLoadGameFunc!"));
			for (UItemObject* Item : SaveData->InventoryItems)
			{
				GetEquipInventoryComponent()->TryAddItem(Item);
				UE_LOG(LogTemp, Warning, TEXT("ItemName : %s"), *Item->GetName());
			}

		}
	}
}

void ASInvenFPSCharacter::SaveGame()
{
	AFPSGameModeBase* GameMode = Cast<AFPSGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		UFPSSaveGame* SaveData = GameMode->GetSaveData();
		if (SaveData == nullptr)
		{
			return;
		}
		//TODO : Character의 인벤, 장비창 세이브 하기
		SaveData->InventoryItems.Empty();
		//TODO : ItemObject를 Copy해서 만들어도 인벤토리에 이상하게 들어간다,
		// 구조체를 만들어서 아이템의 정보를 저장해 사용해야 할것같다.

		for (UItemObject* Item : GetEquipInventoryComponent()->GetInventoryItems())
		{
			UItemObject* CopyItem = Item->CreateItemCopy();
			SaveData->InventoryItems.Add(CopyItem);
		}

		UE_LOG(LogTemp, Warning, TEXT("PlayerSaveGameFunc!"));
		GameMode->SaveGame(SaveData);
	}
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
