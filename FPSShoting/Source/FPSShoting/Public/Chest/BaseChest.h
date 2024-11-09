// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionBase.h"
#include "BaseChest.generated.h"

/**
 * 
 */

class UEquipInventoryComponent;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TObjectPtr<UEquipInventoryComponent> InventoryComponent;

	UFUNCTION(BlueprintImplementableEvent)
	void DisplayChest();
};
