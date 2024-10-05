// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BasicWidgetController.generated.h"

/**
 * 
 */
class UEquipInventoryComponent;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()


		FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController* PC, UEquipInventoryComponent* InvenComp)
		:PlayerController(PC), EquipInventoryComponent(InvenComp) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UEquipInventoryComponent> EquipInventoryComponent = nullptr;
};

UCLASS(BlueprintType, Blueprintable)
class FPSSHOTING_API UBasicWidgetController : public UObject
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);

	UFUNCTION(BlueprintCallable)
	APlayerController* GetPlayerController() { return PlayerController; }

	UFUNCTION(BlueprintCallable)
	UEquipInventoryComponent* GetEquipInventoryComponent() { return EquipInventoryComponent; }


protected:
	/*
	* 인벤토리의 셀 하나당의 크기
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float TileSize = 50.f;

	
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UEquipInventoryComponent> EquipInventoryComponent;

};
