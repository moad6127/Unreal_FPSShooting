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
