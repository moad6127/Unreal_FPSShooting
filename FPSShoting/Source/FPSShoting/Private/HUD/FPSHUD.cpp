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

UBasicWidgetController* AFPSHUD::MakeWidgetController(const FWidgetControllerParams& WCParams)
{
    if (WidgetControllerClass)
    {
        UBasicWidgetController* NewWidgetController = NewObject<UBasicWidgetController>(this, WidgetControllerClass);
        NewWidgetController->SetWidgetControllerParams(WCParams);
        return NewWidgetController;
    }
    return nullptr;
}

void AFPSHUD::InitHUD(APlayerController* PC, UEquipInventoryComponent* SInventoryComponent)
{

    checkf(InventoryWidgetClass, TEXT("Inventory Widget Class uninitialized, fill out BP_FPSHUD"));
    checkf(WidgetControllerClass, TEXT("Inventory Widget Controller Class uninitialized, fill out BP_FPSHUD"));
    checkf(InteractWidgetClass, TEXT("Interact Widget Class uninitialized, fill out BP_FPSHUD"));
    checkf(GamePauseWidgetClass, TEXT("GamePause Widget Class uninitialized, fill out BP_FPSHUD"));
    const FWidgetControllerParams WidgetControllerParmas(PC, SInventoryComponent);

    UBasicWidgetController* BWidgetController = GetWidgetController(WidgetControllerParmas);

    InventoryWidget = CreateWidget<UBasicWidget>(GetWorld(), InventoryWidgetClass);
    InteractWidget = CreateWidget<UBasicWidget>(GetWorld(), InteractWidgetClass);
    GamePauseWidget = CreateWidget<UUserWidget>(GetWorld(), GamePauseWidgetClass);

    InventoryWidget->SetWidgetController(BWidgetController);
    InteractWidget->SetWidgetController(WidgetController);

    InventoryWidget->AddToViewport();
    InteractWidget->AddToViewport();
    GamePauseWidget->AddToViewport();

    InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
    GamePauseWidget->SetVisibility(ESlateVisibility::Hidden);
    bIsInventoryVisible = false;
    bIsPauseVisible = false;
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

void AFPSHUD::ShowGamePause()
{
    if (bIsPauseVisible)
    {
        HideGamePause();
    }
    else
    {
        DisplayGamePause();
    }
}

void AFPSHUD::DrawHUD()
{
    Super::DrawHUD();
    if (!HUDPackage.bDrawCrosshair)
    {
        return;
    }
    FVector2D ViewportSize;
    if (GEngine)
    {
        GEngine->GameViewport->GetViewportSize(ViewportSize);
        const FVector2D ViewportCenter(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
        if (HUDPackage.CrosshairCenter)
        {
            DrawCrosshair(HUDPackage.CrosshairCenter, ViewportCenter);
        }
        if (HUDPackage.CrosshairTop)
        {
            DrawCrosshair(HUDPackage.CrosshairTop, ViewportCenter);
        }
        if (HUDPackage.CrosshairBottom)
        {
            DrawCrosshair(HUDPackage.CrosshairBottom, ViewportCenter);
        }
        if (HUDPackage.CrosshairLeft)
        {
            DrawCrosshair(HUDPackage.CrosshairLeft, ViewportCenter);
        }
        if (HUDPackage.CrosshairRight)
        {
            DrawCrosshair(HUDPackage.CrosshairRight, ViewportCenter);
        }
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

void AFPSHUD::DisplayGamePause()
{
    if (GamePauseWidget)
    {
        bIsPauseVisible = true;
        GamePauseWidget->SetVisibility(ESlateVisibility::Visible);
        const FInputModeGameAndUI InputMode;
        GetOwningPlayerController()->SetInputMode(InputMode);
        GetOwningPlayerController()->SetShowMouseCursor(true);
    }
}

void AFPSHUD::HideGamePause()
{
    if (GamePauseWidget)
    {
        bIsPauseVisible = false;
        GamePauseWidget->SetVisibility(ESlateVisibility::Hidden);
        const FInputModeGameOnly InputMode;
        GetOwningPlayerController()->SetInputMode(InputMode);
        GetOwningPlayerController()->SetShowMouseCursor(false);
    }
}

void AFPSHUD::DrawCrosshair(UTexture2D* CrosshairTexture, FVector2D ViewportCenter)
{
    const float TextureWidth = CrosshairTexture->GetSizeX();
    const float TextureHeight = CrosshairTexture->GetSizeY();
    const FVector2D TextureDrawPoint(
        ViewportCenter.X - (TextureWidth / 2.f),
        ViewportCenter.Y - (TextureHeight / 2.f)
    );

    DrawTexture(
        CrosshairTexture,
        TextureDrawPoint.X,
        TextureDrawPoint.Y,
        TextureWidth,
        TextureHeight,
        0.f,
        0.f,
        1.f,
        1.f,
        FLinearColor::White
    );
}
