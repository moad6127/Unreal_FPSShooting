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

USTRUCT()
struct FItemNumericData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 MaxStackSize;

	UPROPERTY(EditAnywhere)
	int32 ExpandableInventorySize;

	UPROPERTY(EditAnywhere)
	bool bIsStackable;

	UPROPERTY(EditAnywhere)
	bool bIsRotate;

	UPROPERTY(EditAnywhere)
	bool bExpandableSize;

	UPROPERTY(EditAnywhere)
	bool bIsWeapon;
};

USTRUCT()
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

UENUM(BlueprintType)
enum class EEquipmentSlotType : uint8
{
	EEST_None UMETA(DisplayName = "None"),
	EEST_Head UMETA(DisplayName = "Head"),
	EEST_Chest UMETA(DisplayName = "Chest"),
	EEST_Backpack UMETA(DisplayName = "Backpack"),
	EEST_Weapon UMETA(DisplayName = "Weapon"),
	EEST_Weapon1 UMETA(DisplayName = "Weapon1"),
	EEST_Weapon2 UMETA(DisplayName = "Weapon2"),

	EEST_Max UMETA(DisplayName = "DefaultMAX")

};

USTRUCT()
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
};

