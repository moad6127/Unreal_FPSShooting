// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemObject.h"

UItemObject::UItemObject()
{
}

UItemObject* UItemObject::CreateItemCopy()
{
	UItemObject* ItemCopy = NewObject<UItemObject>(StaticClass());
	ItemCopy->ID = ID;
	ItemCopy->SlotType = SlotType;
	ItemCopy->ItemQuantity = ItemQuantity;
	ItemCopy->ItemNumbericData = ItemNumbericData;
	ItemCopy->Asset = Asset;
	ItemCopy->ItemName = ItemName;
	ItemCopy->WeaponData = WeaponData;
	ItemCopy->DataStruct = DataStruct;
	ItemCopy->SetItemSizeX(SizeX);
	ItemCopy->SetItemSizeY(SizeY);
	ItemCopy->SetItemItemLocation(ItemLocation);
	ItemCopy->bIsCopy = true;

	return ItemCopy;
}

void UItemObject::ResetItemFlags()
{
	bIsCopy = false;
	InInventorys = false;
}

void UItemObject::SetItemItemLocation(FIntPoint InItemLocation)
{
	ItemLocation = InItemLocation;
}

void UItemObject::SetItemSizeX(int32 InSize)
{
	SizeX = InSize;
}

void UItemObject::SetItemSizeY(int32 InSize)
{
	SizeY = InSize;
}

void UItemObject::Rotate()
{
	ItemNumbericData.bIsRotate = !ItemNumbericData.bIsRotate;
	Swap(SizeX, SizeY);
}

UTexture2D* UItemObject::GetItemIcon()
{
	if (ItemNumbericData.bIsRotate)
	{
		return Asset.IconRotated;
	}
	return Asset.Icon;
}
