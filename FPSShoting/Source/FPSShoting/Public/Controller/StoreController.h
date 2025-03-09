// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "StoreController.generated.h"

/**
 * 
 */
class UBasicWidget;
class UBasicWidgetController;
class UEquipInventoryComponent;
class UFPSSaveGame;
class UItemObject;

UCLASS()
class FPSSHOTING_API AStoreController : public APlayerController
{
	GENERATED_BODY()
public:

	AStoreController(); 

	UFUNCTION(BlueprintCallable)
	void SaveItems();

	void SaveContainerItems(UFPSSaveGame* SaveData);

	UFUNCTION(BlueprintCallable)
	void BuyItem(UItemObject* BuyItem);

	UFUNCTION(BlueprintCallable)
	void SellItem(UItemObject* SellItem);

	

protected:


	virtual void OnPossess(APawn* aPawn) override;
	virtual void BeginPlay() override;

	void LoadData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UBasicWidget> StoreWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UBasicWidgetController> StoreWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UBasicWidget> StoreWidget;

	UPROPERTY()
	TObjectPtr<UBasicWidgetController> StoreWidgetController;

	UPROPERTY(EditAnywhere)
	TArray<FName> StoreItemNames;

	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<UItemObject>> StoreItems;

	UPROPERTY(BlueprintReadOnly)
	int32 Coins = 0;
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	UEquipInventoryComponent* ContainerInventoryComponent;
};
