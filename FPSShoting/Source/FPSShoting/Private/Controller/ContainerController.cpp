// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/ContainerController.h"
#include "Widget/BasicWidget.h"
#include "Widget/BasicWidgetController.h"
#include "Components/EquipInventoryComponent.h"

AContainerController::AContainerController()
{
	ContainerInventoryComponent = CreateDefaultSubobject<UEquipInventoryComponent>("EquipInventory");
}

void AContainerController::BeginPlay()
{
	Super::BeginPlay();
	FWidgetControllerParams ControllerParams(this, ContainerInventoryComponent);

	if (ContainerWidgetControllerClass)
	{
		ContainerWidgetController = NewObject<UBasicWidgetController>(this, ContainerWidgetControllerClass);
		if (ContainerWidgetController)
		{
			ContainerWidgetController->SetWidgetControllerParams(ControllerParams);
			if (ContainerWidgetClass)
			{
				ContainerWidget = CreateWidget<UBasicWidget>(this, ContainerWidgetClass);
				if (ContainerWidget)
				{
					ContainerWidget->SetWidgetController(ContainerWidgetController);
					ContainerWidget->AddToViewport();
					bShowMouseCursor = true;
				}
			}
		}
	}
}
