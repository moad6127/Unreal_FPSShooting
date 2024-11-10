// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FPSHUD.generated.h"

/**
 * 
 */
class UBasicWidget;
struct FWidgetControllerParams;
class UEquipInventoryComponent;
class UBasicWidgetController;

USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()
public:
	bool bDrawCrosshair;
	class UTexture2D* CrosshairCenter;
	UTexture2D* CrosshairLeft;
	UTexture2D* CrosshairRight;
	UTexture2D* CrosshairTop;
	UTexture2D* CrosshairBottom;
};

UCLASS()
class FPSSHOTING_API AFPSHUD : public AHUD
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	UBasicWidgetController* GetWidgetController(const FWidgetControllerParams& WCParams);

	UFUNCTION(BlueprintCallable)
	UBasicWidgetController* MakeWidgetController(const FWidgetControllerParams& WCParams);

	void InitHUD(APlayerController* PC, UEquipInventoryComponent* SInventoryComponent);

	void ShowInventory();
	virtual void DrawHUD() override;
	void SetHUDPackage(const FHUDPackage& InHUDPackage) { HUDPackage = InHUDPackage; }
protected:

	TObjectPtr<UBasicWidget> InventoryWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBasicWidget>InventoryWidgetClass;

	TObjectPtr<UBasicWidgetController> WidgetController;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBasicWidgetController> WidgetControllerClass;

	TObjectPtr<UBasicWidget> InteractWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBasicWidget>InteractWidgetClass;

	FHUDPackage HUDPackage;

	bool bIsInventoryVisible;

	void DisplayInventory();
	void HideInventory();

	void DrawCrosshair(UTexture2D* CrosshairTexture, FVector2D ViewportCenter);
};
