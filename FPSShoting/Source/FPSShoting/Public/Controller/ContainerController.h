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

UCLASS()
class FPSSHOTING_API AContainerController : public APlayerController
{
	GENERATED_BODY()

public:

	AContainerController();

	UFUNCTION(BlueprintCallable)
	UEquipInventoryComponent* GetPlayerInventoryComponent() { return PlayerInventoryComponent; }
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
