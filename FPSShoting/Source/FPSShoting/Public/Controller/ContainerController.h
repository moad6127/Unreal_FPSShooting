// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ContainerController.generated.h"

/**
 * 
 */

class UBasicWidget;
class UBasicWidgetController;
class UEquipInventoryComponent;
class UFPSSaveGame;

UCLASS()
class FPSSHOTING_API AContainerController : public APlayerController
{
	GENERATED_BODY()

public:

	AContainerController();

	UFUNCTION(BlueprintCallable)
	void SaveItems();

	void SaveCharacterItems(UFPSSaveGame* SaveData);
	void SaveCharacterEquips(UFPSSaveGame* SaveData);

	void SaveContainerItems(UFPSSaveGame* SaveData);

	UFUNCTION(BlueprintCallable)
	UEquipInventoryComponent* GetPlayerInventoryComponent() { return PlayerInventoryComponent; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UBasicWidgetController* GetPlayerInventoryWidgetController() { return PlayerInventoryWidgetController; }
protected:
	
	virtual void OnPossess(APawn* aPawn) override;
	virtual void BeginPlay() override;

	void LoadData();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UBasicWidget> ContainerWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UBasicWidgetController> ContainerWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UBasicWidget> ContainerWidget;

	UPROPERTY()
	TObjectPtr<UBasicWidgetController> ContainerWidgetController;

	UPROPERTY()
	TObjectPtr<UBasicWidgetController> PlayerInventoryWidgetController;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	UEquipInventoryComponent* ContainerInventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	UEquipInventoryComponent* PlayerInventoryComponent;
};
