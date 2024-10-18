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
	* Item을 추가시키기 위한 함수
	*/
	bool TryAddItem(UItemObject* InItem);

	/*
	* 아이템을 인벤토리에서 제거하는 함수
	*/
	bool RemoveItems(UItemObject* InItem);

	/*
	* 아이템을 Drop할때 사용되는 함수
	*/
	void DropItem(UItemObject* ItemToDrop);

	/*	
	* 아이템을 인벤토리의 다른 공간으로 이동시키는 함수
	*/
	bool ReplaceItem(UItemObject* ItemToReplace, FIntPoint InLocation);

	/*
	*  아이템을 회전시키는 함수
	*/
	void RotateItem(UItemObject* ItemToRotate);

	/*
	* 인벤토리에 아이템을 넣을때 넣을수 있는 공간이 존재 하는지 확인하는 함수
	*/
	bool IsRoomAvailable(UItemObject* InItem, FIntPoint InLocation);

	/*
	* 아이템 장착 함수
	*/
	void EquipItem(UItemObject* InItem);
	/*
	* 아이템 장착 해제 함수
	*/
	void UnEquipItem(UItemObject* InItem);

	/*
	* 아이템을 소비/제거 하는 함수
	*/
	void ConsumeItem(UItemObject* InItem, int32 ConsumeAmount);

	/*
	* Stackable아이템에 아이템을 더하는 함수
	*/
	int32 AddItemAmount(UItemObject* InItem, int32 AddAmount);
	/*
	* 아이템을 half로 나누는 함수
	*/
	void SplitItem(UItemObject* InItem);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	/*
	* 아이템이 들어왔을때 InventoryGrid에 1을 넣어서 공간 채우기
	*/
	void PlaceItem(UItemObject* InItem, FIntPoint InLocation);

	/*
	* 아이템을 제거할때 InventoryGrid공간을 아이템 공간만큼 초기화 시키기
	*/
	void RemovePlaceItem(UItemObject* InItem);


	/*
	* 인벤토리Grid초기화
	*/ 
	UFUNCTION(BlueprintCallable, Category = "InventoryInit")
	void InitializeInventory();

	/*
	* 아이템의 BottomRight가 인벤토리 공간 범위를 넘어가는지 확인하기
	*/
	bool IsPositionValid(FIntPoint InLocation);

	void HandleEquip(UItemObject* InItem);
	bool HandleAddItem(UItemObject* InItem);
	bool HandleStackableItem(UItemObject* InItem);

	/*
	* 총알을 획득하거나 버릴 경우 Ammo의 Amount를 제거한다.
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
