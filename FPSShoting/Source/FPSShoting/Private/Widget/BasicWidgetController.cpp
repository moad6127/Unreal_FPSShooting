// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/BasicWidgetController.h"
#include "Components/EquipInventoryComponent.h"
#include "Items/ItemObject.h"

void UBasicWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	EquipInventoryComponent = WCParams.EquipInventoryComponent;
}

bool UBasicWidgetController::TryAddItem(UItemObject* InItem)
{
	if (GetEquipInventoryComponent()->TryAddItem(InItem))
	{
		return true;
	}
	return false;
}

bool UBasicWidgetController::IsRoomAvailable(UItemObject* Payload, FIntPoint Location)
{
	if (GetEquipInventoryComponent())
	{
		if (GetEquipInventoryComponent()->IsRoomAvailable(Payload, Location))
		{
			return true;
		}
	}
	return false;
}

bool UBasicWidgetController::RemoveItem(UItemObject* InItem)
{
	if (GetEquipInventoryComponent()&& GetEquipInventoryComponent()->RemoveItems(InItem))
	{
		return true;
	}
	return false;
}

void UBasicWidgetController::DropItem(UItemObject* ItemToDrop)
{
	if (GetEquipInventoryComponent())
	{
		GetEquipInventoryComponent()->DropItem(ItemToDrop);
	}
}

bool UBasicWidgetController::ReplaceItem(UItemObject* ItemToReplace, FIntPoint InLocation)
{
	if (GetEquipInventoryComponent())
	{
		if (GetEquipInventoryComponent()->ReplaceItem(ItemToReplace, InLocation))
		{
			return true;
		}
	}
	return false;
}

void UBasicWidgetController::RotateItem(UItemObject* ItemToRotate)
{
	if (GetEquipInventoryComponent())
	{
		GetEquipInventoryComponent()->RotateItem(ItemToRotate);
	}
}

void UBasicWidgetController::EquipItem(UItemObject* InItem)
{
	if (GetEquipInventoryComponent())
	{
		GetEquipInventoryComponent()->EquipItem(InItem);
	}
}

bool UBasicWidgetController::UnEquipItem(UItemObject* ItemToUnEquip)
{
	if (GetEquipInventoryComponent())
	{
		return GetEquipInventoryComponent()->UnEquipItem(ItemToUnEquip);
	}
	return false;
}

void UBasicWidgetController::SplitItem(UItemObject* SplitToItem)
{
	if (GetEquipInventoryComponent())
	{
		GetEquipInventoryComponent()->SplitItem(SplitToItem);
	}
}

void UBasicWidgetController::MousePositionInTile(const FVector2D MousePosition, bool& Right, bool& Down)
{
	Right = false;
	Down = false;

	float MousePositionX = MousePosition.X;
	float MousePositionY = MousePosition.Y;

	Right = fmod(MousePositionX, TileSize) > (TileSize / 2);
	Down = fmod(MousePositionY, TileSize) > (TileSize / 2);
}
