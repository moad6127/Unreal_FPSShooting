// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/BasicWidgetController.h"

void UBasicWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	EquipInventoryComponent = WCParams.EquipInventoryComponent;
}
