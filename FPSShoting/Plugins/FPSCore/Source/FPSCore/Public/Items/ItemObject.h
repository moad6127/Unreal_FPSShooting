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
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FName ID;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Data Table")
	int32 ItemQuantity;

	/*
	* EquipmentSlotType
	*/
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	EEquipmentSlotType SlotType;

	/*
	* NumbericData : StackSize, ExpandableSize(more InventorySize plus), bRotate ...
	*/
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	FItemNumericData ItemNumbericData;

	/*
	*  Mesh, ItemIcon ..
	*/
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	FItemAssetData Asset;

	/*
	* Item Name(Use Widget Text)
	*/

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	FText ItemName;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	FWeaponData WeaponData;

	/** Local weapon data struct to keep track of ammo amounts and weapon health */
	UPROPERTY(Replicated, BlueprintReadOnly)
	FRuntimeWeaponData DataStruct;

	bool bIsCopy = false;

	bool InInventorys = false;



#pragma region FUNCTIONS
	UItemObject();

	UItemObject* CreateItemCopy();

	void ResetItemFlags();

	virtual bool IsSupportedForNetworking() const { return true; }
	virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty >& OutLifetimeProps) const;

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

	bool operator==(const FName& OtherID) const
	{
		return ID == OtherID;
	}
#pragma endregion
protected:



private:

	/*
	* Inventory Size(X * Y)
	*/
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "ItemInfo", meta = (AllowPrivateAccess = "true"))
	int32 SizeX;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "ItemInfo", meta = (AllowPrivateAccess = "true"))
	int32 SizeY;


	/*
	* Inventory Item Location (X , Y) Position
	*/
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "ItemInfo", meta = (AllowPrivateAccess = "true"))
	FIntPoint ItemLocation;
};
