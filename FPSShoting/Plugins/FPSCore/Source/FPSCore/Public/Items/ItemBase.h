// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "InteractionBase.h"
#include "ItemBase.generated.h"

/**
 * 
 */

class USphereComponent;
class UItemObject;
class UDataTable;

UCLASS()
class FPSCORE_API AItemBase : public AInteractionBase
{
	GENERATED_BODY()

public:
	AItemBase();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void UpdateInteractionPopupText(const FText NewInteractionText) { InteractionText = NewInteractionText; }

	virtual void Interact() override;

	void InitializeDrop(UItemObject* ItemToDrop);

	void InitializeItem(const TSubclassOf<UItemObject> BaseItem);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(BlueprintReadOnly, Category = "Data Table")
	TObjectPtr<UItemObject> ItemObject;

	/*
	* WeaponData
	*/

	/** Weapon to spawn when picked up */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Data Table")
	TSubclassOf<AWeaponBase> WeaponReference;

	/** Data table reference for weapon (used to see if the weapon has attachments) */
	UPROPERTY(EditDefaultsOnly, Category = "Data Table")
	UDataTable* WeaponDataTable;

	/** Local weapon data struct to keep track of ammo amounts and weapon health */
	UPROPERTY()
	FRuntimeWeaponData DataStruct;

	/*
	* Ammo Data
	*/

	/** The type of ammo that this instance should have */
	UPROPERTY(EditAnywhere, Category = "Ammo Data Table")
	EAmmoType AmmoType;

	/*
	* Base Item Data
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Table")
	TObjectPtr<UDataTable> ItemDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Table")
	FName DesiredItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Table")
	int32 ItemQuantity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Table")
	int32 ItemSizeX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Table")
	int32 ItemSizeY;

private:

	void HandleInteract();
};
