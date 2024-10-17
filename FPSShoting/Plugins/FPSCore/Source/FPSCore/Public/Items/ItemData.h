// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"

USTRUCT(BlueprintType)
struct FLine
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D Start = FVector2D();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D End = FVector2D();
};

USTRUCT(BlueprintType)
struct FItemNumericData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxStackSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ExpandableInventorySize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemQuantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsStackable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsRotate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bExpandableSize;
};

/** Enumerator holding the 4 types of ammunition that weapons can use (used as part of the FSingleWeaponParams struct)
 * and to keep track of the total ammo the player has (ammoMap) */
UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	Pistol       UMETA(DisplayName = "Pistol Ammo"),
	Rifle        UMETA(DisplayName = "Rifle Ammo"),
	Shotgun      UMETA(DisplayName = "Shotgun Ammo"),
	Special		 UMETA(DisplayName = "Special Ammo"),
};

USTRUCT(BlueprintType)
struct FItemAssetData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere)
	UTexture2D* IconRotated;

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh;
};

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	bool bIsWeapon;

	/** Weapon to spawn when picked up */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (EditCondition = "bIsWeapon == true"))
	TSubclassOf<class AWeaponBase> WeaponReference;

	/** Data table reference for weapon (used to see if the weapon has attachments) */
	UPROPERTY(EditDefaultsOnly, Category = "Data Table", meta = (EditCondition = "bIsWeapon == true"))
	UDataTable* WeaponDataTable;

	/*
	* AmmoData
	*/

	/** The type of ammo that this instance should have */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Properties")
	EAmmoType AmmoType;
};

UENUM(BlueprintType)
enum class EEquipmentSlotType : uint8
{
	EEST_None UMETA(DisplayName = "None"),
	EEST_Head UMETA(DisplayName = "Head"),
	EEST_Chest UMETA(DisplayName = "Chest"),
	EEST_Backpack UMETA(DisplayName = "Backpack"),
	EEST_Ammo UMETA(DisplayName = "Ammo"),
	EEST_Weapon UMETA(DisplayName = "Weapon"),
	EEST_Weapon1 UMETA(DisplayName = "Weapon1"),
	EEST_Weapon2 UMETA(DisplayName = "Weapon2"),

	EEST_Max UMETA(DisplayName = "DefaultMAX")

};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FName ID;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EEquipmentSlotType SlotType;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemNumericData ItemNumbericData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemAssetData Asset;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FText ItemName;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FWeaponData WeaponData;
};

