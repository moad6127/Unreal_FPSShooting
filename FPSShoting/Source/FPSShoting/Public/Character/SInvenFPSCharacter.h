// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPSCharacter.h"
#include "SInvenFPSCharacter.generated.h"

/**
 * 
 */
class AFPSHUD;
class UEquipInventoryComponent;

UCLASS()
class FPSSHOTING_API ASInvenFPSCharacter : public AFPSCharacter
{
	GENERATED_BODY()
public:


protected:
	ASInvenFPSCharacter();

	virtual void BeginPlay() override;

	void ShowInventory();

	UPROPERTY()
	AFPSHUD* HUD;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	UEquipInventoryComponent* SInventoryComponent;
};
