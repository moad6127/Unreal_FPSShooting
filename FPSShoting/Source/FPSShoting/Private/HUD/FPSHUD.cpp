// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/FPSHUD.h"
#include "Widget/BasicWidgetController.h"
#include "Widget/BasicWidget.h"

UBasicWidgetController* AFPSHUD::GetWidgetController(const FWidgetControllerParams& WCParams)
{
    if (WidgetController == nullptr)
    {
        WidgetController = NewObject<UBasicWidgetController>(this, WidgetControllerClass);
        WidgetController->SetWidgetControllerParams(WCParams);
    }
    return WidgetController;
}

void AFPSHUD::InitHUD(APlayerController* PC, UEquipInventoryComponent* SInventoryComponent)
{

    checkf(InventoryWidgetClass, TEXT("Inventory Widget Class uninitialized, fill out BP_FPSHUD"));
    checkf(WidgetControllerClass, TEXT("Inventory Widget Controller Class uninitialized, fill out BP_FPSHUD"));
    checkf(InteractWidgetClass, TEXT("Interact Widget Class uninitialized, fill out BP_FPSHUD"));

    const FWidgetControllerParams WidgetControllerParmas(PC, SInventoryComponent);

    UBasicWidgetController* BWidgetController = GetWidgetController(WidgetControllerParmas);

    InventoryWidget = CreateWidget<UBasicWidget>(GetWorld(), InventoryWidgetClass);
    InteractWidget = CreateWidget<UBasicWidget>(GetWorld(), InteractWidgetClass);

    InventoryWidget->SetWidgetController(BWidgetController);
    InteractWidget->SetWidgetController(WidgetController);

    InventoryWidget->AddToViewport();
    InteractWidget->AddToViewport();

    InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
    bIsInventoryVisible = false;
}

void AFPSHUD::ShowInventory()
{
    if (bIsInventoryVisible)
    {
        HideInventory();
    }
    else
    {
        DisplayInventory();
    }
}

void AFPSHUD::DisplayInventory()
{
    if (InventoryWidget)
    {
        bIsInventoryVisible = true;
        InventoryWidget->SetVisibility(ESlateVisibility::Visible);
        const FInputModeGameAndUI InputMode;
        GetOwningPlayerController()->SetInputMode(InputMode);
        GetOwningPlayerController()->SetShowMouseCursor(true);
    }       
}

void AFPSHUD::HideInventory()
{
    if (InventoryWidget)
    {
        bIsInventoryVisible = false;
        InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
        const FInputModeGameOnly InputMode;
        GetOwningPlayerController()->SetInputMode(InputMode);
        GetOwningPlayerController()->SetShowMouseCursor(false);
    }
}
