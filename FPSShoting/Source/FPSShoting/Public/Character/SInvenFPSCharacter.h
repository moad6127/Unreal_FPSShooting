// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPSCharacter.h"
#include "SInvenFPSCharacter.generated.h"

/**
 * 
 */
class AFPSHUD;

UCLASS()
class FPSSHOTING_API ASInvenFPSCharacter : public AFPSCharacter
{
	GENERATED_BODY()
public:


protected:
	ASInvenFPSCharacter();

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ShowInventory();

	/* Crosshair Texture Set */
	void SetHUDCrosshair(float DeltaTime);

	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	AFPSHUD* HUD;

	UPROPERTY(EditDefaultsOnly, Category = "Input | Actions")
	UInputAction* InventoryAction;
};
