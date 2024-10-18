// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipInventoryComponent.generated.h"

struct FTile;
class UItemObject;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventorySizeChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemEquipChanged, EEquipmentSlotType, EquipSlot);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSCORE_API UEquipInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEquipInventoryComponent();

	UPROPERTY(BlueprintAssignable, Category = "SInventoryComponent")
	FInventoryChanged InventoryChanged;

	UPROPERTY(BlueprintAssignable, Category = "SInventoryComponent")
	FInventorySizeChanged InventorySizeChanged;

	UPROPERTY(BlueprintAssignable, Category = "SInventoryComponent")
	FItemEquipChanged ItemEquipChanged;

	UFUNCTION(BlueprintCallable, Category = "SInventoryComponent")
	TArray<UItemObject*> GetInventoryItems() const { return InventoryItems; }

	UFUNCTION(BlueprintCallable, Category = "SInventoryComponent")
	TMap<EEquipmentSlotType, UItemObject*> GetEquipmentItems() const { return EquipmentItems; }

	/*
	* Item�� �߰���Ű�� ���� �Լ�
	*/
	bool TryAddItem(UItemObject* InItem);

	/*
	* �������� �κ��丮���� �����ϴ� �Լ�
	*/
	bool RemoveItems(UItemObject* InItem);

	/*
	* �������� Drop�Ҷ� ���Ǵ� �Լ�
	*/
	void DropItem(UItemObject* ItemToDrop);

	/*	
	* �������� �κ��丮�� �ٸ� �������� �̵���Ű�� �Լ�
	*/
	bool ReplaceItem(UItemObject* ItemToReplace, FIntPoint InLocation);

	/*
	*  �������� ȸ����Ű�� �Լ�
	*/
	void RotateItem(UItemObject* ItemToRotate);

	/*
	* �κ��丮�� �������� ������ ������ �ִ� ������ ���� �ϴ��� Ȯ���ϴ� �Լ�
	*/
	bool IsRoomAvailable(UItemObject* InItem, FIntPoint InLocation);

	/*
	* ������ ���� �Լ�
	*/
	void EquipItem(UItemObject* InItem);
	/*
	* ������ ���� ���� �Լ�
	*/
	void UnEquipItem(UItemObject* InItem);

	/*
	* �������� �Һ�/���� �ϴ� �Լ�
	*/
	void ConsumeItem(UItemObject* InItem, int32 ConsumeAmount);

	/*
	* Stackable�����ۿ� �������� ���ϴ� �Լ�
	*/
	int32 AddItemAmount(UItemObject* InItem, int32 AddAmount);
	/*
	* �������� half�� ������ �Լ�
	*/
	void SplitItem(UItemObject* InItem);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	/*
	* �������� �������� InventoryGrid�� 1�� �־ ���� ä���
	*/
	void PlaceItem(UItemObject* InItem, FIntPoint InLocation);

	/*
	* �������� �����Ҷ� InventoryGrid������ ������ ������ŭ �ʱ�ȭ ��Ű��
	*/
	void RemovePlaceItem(UItemObject* InItem);


	/*
	* �κ��丮Grid�ʱ�ȭ
	*/ 
	UFUNCTION(BlueprintCallable, Category = "InventoryInit")
	void InitializeInventory();

	/*
	* �������� BottomRight�� �κ��丮 ���� ������ �Ѿ���� Ȯ���ϱ�
	*/
	bool IsPositionValid(FIntPoint InLocation);

	void HandleEquip(UItemObject* InItem);
	bool HandleAddItem(UItemObject* InItem);
	bool HandleStackableItem(UItemObject* InItem);

	/*
	* �Ѿ��� ȹ���ϰų� ���� ��� Ammo�� Amount�� �����Ѵ�.
	*/
	void AddAmmo(UItemObject* InItem);
	void RemoveAmmo(UItemObject* InItem);




	int32 GetIndex(int32 x, int32 y) const { return y * Columns + x; };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constants", meta = (AllowPrivateAccess = "true"))
	int32 Columns = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constants", meta = (AllowPrivateAccess = "true"))
	int32 Rows = 5;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UItemObject>> InventoryItems;

	UPROPERTY(VisibleAnywhere, Category = "Equipment", meta = (AllowPrivateAccess = "true"))
	TMap<EEquipmentSlotType, UItemObject*> EquipmentItems;

	UPROPERTY()
	TArray<bool> InventoryGrid;
		
};
