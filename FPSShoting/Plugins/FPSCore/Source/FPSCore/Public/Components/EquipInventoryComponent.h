// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipInventoryComponent.generated.h"

struct FTile;
class UItemObject;
class UDataTable;
class UFPSSaveGame;
class AItemBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventorySizeChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemEquipChanged, EEquipmentSlotType, EquipSlot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTryAddItem, UItemObject*, AddItem);
USTRUCT()
struct FStarterItemData 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Data Table")
	FName ItemName;

	UPROPERTY(EditAnywhere, Category = "Data Table")
	UDataTable* ItemDataTable;

	UPROPERTY(EditAnywhere, Category = "Data Table")
	int32 ItemQuantity;
};

USTRUCT(BlueprintType)
struct FEquipmentItems
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UItemObject> Head = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UItemObject> Body = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UItemObject> BackPack = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UItemObject> Weapon1 = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UItemObject> Weapon2 = nullptr;

};


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

	UPROPERTY(BlueprintAssignable, Category = "SInventoryComponent")
	FTryAddItem OnTryAddItem;

	UFUNCTION(BlueprintCallable, Category = "SInventoryComponent")
	TArray<UItemObject*> GetInventoryItems() const { return InventoryItems; }

	UFUNCTION(BlueprintCallable, Category = "SInventoryComponent")
	FEquipmentItems GetEquipmentItems() const { return EquipmentItems; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags);

	void AddLoadedEquipItem(UItemObject* InItem);

	void AddLoadedInventoryItem(UItemObject* InItem);
	/*
	* Item을 추가시키기 위한 함수
	*/
	bool TryAddItem(UItemObject* InItem);

	/*
	* Inventory를 Load할때 사용할 함수
	*/
	bool AddItem(UItemObject* InItem);

	/*
	* 아이템을 인벤토리에서 제거하는 함수
	*/
	bool RemoveItems(UItemObject* InItem);

	UFUNCTION(Server, Reliable)
	void ServerRemoveItems(UItemObject* InItem);

	/*
	* 아이템을 Drop할때 사용되는 함수
	*/
	void DropItem(UItemObject* ItemToDrop);
	
	UFUNCTION(Server, Reliable)
	void ServerDropItem(UItemObject* ItemToDrop);

	/*	
	* 아이템을 인벤토리의 다른 공간으로 이동시키는 함수
	*/
	bool ReplaceItem(UItemObject* ItemToReplace, FIntPoint InLocation);

	bool HandleReplaceItem(UItemObject* ItemToReplace, FIntPoint InLocation);

	UFUNCTION(Server, Reliable)
	void ServerReplaceItem(UItemObject* ItemToReplace, FIntPoint InLocation);

	UFUNCTION(Client, Reliable)
	void ClientReplaceItemRespons(UItemObject* ItemToReplace, bool bWasSuccessful);
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
	UFUNCTION(Server, Reliable)
	void ServerEquipItem(UItemObject* InItem);

	void EquipItem(UItemObject* InItem);
	/*
	* 아이템 장착 해제 함수
	*/
	UFUNCTION(Server, Reliable)
	void ServerUnEquipItem(UItemObject* InItem);

	bool UnEquipItem(UItemObject* InItem);

	bool HandleUnEquipItem(UItemObject* InItem);
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

	void HandleSplitItem(UItemObject* InItem, int32 OriginalQuantity);

	UFUNCTION(Server, Reliable)
	void ServerSplitItem(UItemObject* InItem, int32 OriginalQuantity);


	/*
	* 인벤토리Grid초기화
	*/
	UFUNCTION(BlueprintCallable, Category = "InventoryInit")
	void InitializeInventory();


	/*
	* 해당 슬롯의 장비를 Get하는 함수
	*/
	UFUNCTION(BlueprintCallable, Category = "Eqiupment")
	UItemObject* GetEquipItemToSlot(EEquipmentSlotType SlotType);
	
	/*
	* 장비를 장착할때 사용되는 함수
	*/
	void SetEquipmentItem(UItemObject* InItem);

	/*
	* 장비를 해제할때 사용되는 함수
	*/
	void RemoveEquipmentItem(EEquipmentSlotType SlotType);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void StarterItem();

	void LoadedItemEquip();

	void LoadedInventoryItem();
private:

	/*
	* 아이템이 들어왔을때 InventoryGrid에 1을 넣어서 공간 채우기
	*/
	void PlaceItem(UItemObject* InItem, FIntPoint InLocation);

	/*
	* 아이템을 제거할때 InventoryGrid공간을 아이템 공간만큼 초기화 시키기
	*/
	void RemovePlaceItem(UItemObject* InItem);

	bool CheckPlace(int32 ExpandableSize);



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

	//Replace를 할때 Item이 Ammo일경우 인벤토리의 모든 Ammo체크하기
	void CheckAmmo(UItemObject* InItem);

	UFUNCTION()
	void OnRep_InventorySizeChanged();

	UFUNCTION()
	void OnRep_InventoryItems();

	UFUNCTION()
	void OnRep_EquipmentItems(FEquipmentItems LastItems);

	int32 GetIndex(int32 x, int32 y) const { return y * Columns + x; };

	UPROPERTY(ReplicatedUsing = OnRep_InventorySizeChanged, EditAnywhere, BlueprintReadOnly, Category = "Constants", meta = (AllowPrivateAccess = "true"))
	int32 Columns = 5;

	UPROPERTY(ReplicatedUsing = OnRep_InventorySizeChanged,EditAnywhere, BlueprintReadOnly, Category = "Constants", meta = (AllowPrivateAccess = "true"))
	int32 Rows = 5;

	UPROPERTY(ReplicatedUsing = OnRep_InventoryItems, VisibleAnywhere, BlueprintReadOnly, Category = "Items", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UItemObject>> InventoryItems;

	UPROPERTY(ReplicatedUsing = OnRep_EquipmentItems, VisibleAnywhere, Category = "Equipment")
	FEquipmentItems EquipmentItems;

	UPROPERTY(Replicated)
	TArray<bool> InventoryGrid;

	UPROPERTY(EditAnywhere, Category = "Items")
	TArray<FStarterItemData> StartItems;

	UPROPERTY()
	TArray<TObjectPtr<UItemObject>> LoadedEquipItems;

	UPROPERTY()
	TArray<TObjectPtr<UItemObject>> LoadedInventoryItems;
};
