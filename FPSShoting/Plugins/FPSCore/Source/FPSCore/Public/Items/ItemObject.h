// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemData.h"
#include "WeaponBase.h"
#include "ItemObject.generated.h"

/**
 * 
 */

UCLASS(BlueprintType, Blueprintable)
class FPSCORE_API UItemObject : public UObject
{
	GENERATED_BODY()

public:
	/*
	* ItemObejct ID
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FName ID;

	/*
	* EquipmentSlotType
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	EEquipmentSlotType SlotType;

	/*
	* NumbericData : StackSize, ExpandableSize(more InventorySize plus), bRotate ...
	*/
	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemNumericData ItemNumbericData;

	/*
	*  Mesh, ItemIcon ..
	*/
	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemAssetData Asset;

	/*
	* Item Name(Use Widget Text)
	*/

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FText ItemName;

	/** Weapon to spawn when picked up */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AWeaponBase> WeaponReference;

	/** Data table reference for weapon (used to see if the weapon has attachments) */
	UPROPERTY(EditDefaultsOnly, Category = "Data Table")
	UDataTable* WeaponDataTable;

	/** Local weapon data struct to keep track of ammo amounts and weapon health */
	UPROPERTY(BlueprintReadOnly)
	FRuntimeWeaponData DataStruct;


#pragma region FUNCTIONS
	UItemObject();

	UItemObject* CreateItemCopy();

	FIntPoint GetItemItemLocation() const { return ItemLocation; }
	int32 GetSizeX() const { return SizeX; }
	int32 GetSizeY() const { return SizeY; };
	void SetItemItemLocation(FIntPoint InItemLocation);
	void SetItemSizeX(int32 InSize);
	void SetItemSizeY(int32 InSize);

	UFUNCTION(BlueprintCallable)
	FText GetItemName() const { return ItemName; }

	UFUNCTION(BlueprintCallable)
	bool GetRotateState() const { return ItemNumbericData.bIsRotate; }

	void Rotate();

	UFUNCTION(BlueprintCallable)
	UTexture2D* GetItemIcon();
#pragma endregion
protected:

	bool operator==(const FName& OtherID) const
	{
		return ID == OtherID;
	}

private:

	/*
	* Inventory Size(X * Y)
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemInfo", meta = (AllowPrivateAccess = "true"))
	int32 SizeX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemInfo", meta = (AllowPrivateAccess = "true"))
	int32 SizeY;

	/*
	* Inventory Item Location (X , Y) Position
	*/
	UPROPERTY(BlueprintReadOnly, Category = "ItemInfo", meta = (AllowPrivateAccess = "true"))
	FIntPoint ItemLocation;
};
