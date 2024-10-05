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

UCLASS()
class FPSSHOTING_API AFPSHUD : public AHUD
{
	GENERATED_BODY()
public:
	UBasicWidgetController* GetWidgetController(const FWidgetControllerParams& WCParams);

	void InitHUD(APlayerController* PC, UEquipInventoryComponent* SInventoryComponent);

	void ShowInventory();
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

	bool bIsInventoryVisible;

	void DisplayInventory();
	void HideInventory();
};
