// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemObject.h"
#include "Net/UnrealNetwork.h"

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

void UItemObject::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UItemObject, ID);
	DOREPLIFETIME(UItemObject, ItemQuantity);
	DOREPLIFETIME(UItemObject, SlotType);
	DOREPLIFETIME(UItemObject, ItemNumbericData);
	DOREPLIFETIME(UItemObject, Asset);
	DOREPLIFETIME(UItemObject, ItemName);
	DOREPLIFETIME(UItemObject, WeaponData);
	DOREPLIFETIME(UItemObject, DataStruct);
	DOREPLIFETIME(UItemObject, SizeX);
	DOREPLIFETIME(UItemObject, SizeY);
	DOREPLIFETIME(UItemObject, ItemLocation);
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
