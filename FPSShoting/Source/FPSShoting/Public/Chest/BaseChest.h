// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionBase.h"
#include "BaseChest.generated.h"

/**
 * 
 */

class UEquipInventoryComponent;
class UBasicWidgetController;
class UBasicWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDisplayChest);

UCLASS()
class FPSSHOTING_API ABaseChest : public AInteractionBase
{
	GENERATED_BODY()

public:

	ABaseChest();

	virtual void Interact() override;

	UFUNCTION(BlueprintCallable)
	UEquipInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }
protected:
	UFUNCTION(Client, Reliable)
	void ClientDisplayChest();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TObjectPtr<UEquipInventoryComponent> InventoryComponent;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UBasicWidget> ChestWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBasicWidget> ChestWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBasicWidgetController> WidgetControllerClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UBasicWidgetController> CharcterWidgetController;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UBasicWidgetController> ChestWidgetController;

	UPROPERTY(BlueprintAssignable, Category = "Chest")
	FDisplayChest DisplayChestBox;

	UFUNCTION(BlueprintImplementableEvent)
	void DisplayChest();
};
