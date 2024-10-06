// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemBase")
	TObjectPtr<UItemObject> ItemObject;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemBase")
	TObjectPtr<UDataTable> ItemDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemBase")
	FName DesiredItemID;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemBase")
	//int32 ItemQuantity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemBase")
	int32 ItemSizeX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemBase")
	int32 ItemSizeY;

private:
	void InitializeItem(const TSubclassOf<UItemObject> BaseItem);
};
