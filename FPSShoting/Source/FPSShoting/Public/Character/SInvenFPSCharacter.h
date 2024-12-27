// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPSCharacter.h"
#include "SInvenFPSCharacter.generated.h"

/**
 * 
 */
class AFPSHUD;
class UFPSSaveGame;

UCLASS()
class FPSSHOTING_API ASInvenFPSCharacter : public AFPSCharacter
{
	GENERATED_BODY()
public:
	virtual void PossessedBy(AController* NewController) override;

	void SaveGame();

	UFUNCTION(BlueprintCallable)
	AFPSHUD* GetHUD() const;
protected:
	ASInvenFPSCharacter();

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ShowInventory();

	/* Crosshair Texture Set */
	void SetHUDCrosshair(float DeltaTime);

	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	// 게임 load하기
	void LoadGame();

	void EquipItemSave(UFPSSaveGame* SaveData);

	UPROPERTY()
	AFPSHUD* HUD;

	UPROPERTY(EditDefaultsOnly, Category = "Input | Actions")
	UInputAction* InventoryAction;
};
