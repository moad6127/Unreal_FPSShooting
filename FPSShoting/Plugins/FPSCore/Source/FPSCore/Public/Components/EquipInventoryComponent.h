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
	* Item�� �߰���Ű�� ���� �Լ�
	*/
	bool TryAddItem(UItemObject* InItem);

	/*
	* Inventory�� Load�Ҷ� ����� �Լ�
	*/
	bool AddItem(UItemObject* InItem);

	/*
	* �������� �κ��丮���� �����ϴ� �Լ�
	*/
	bool RemoveItems(UItemObject* InItem);

	UFUNCTION(Server, Reliable)
	void ServerRemoveItems(UItemObject* InItem);

	/*
	* �������� Drop�Ҷ� ���Ǵ� �Լ�
	*/
	void DropItem(UItemObject* ItemToDrop);
	
	UFUNCTION(Server, Reliable)
	void ServerDropItem(UItemObject* ItemToDrop);

	/*	
	* �������� �κ��丮�� �ٸ� �������� �̵���Ű�� �Լ�
	*/
	bool ReplaceItem(UItemObject* ItemToReplace, FIntPoint InLocation);

	bool HandleReplaceItem(UItemObject* ItemToReplace, FIntPoint InLocation);

	UFUNCTION(Server, Reliable)
	void ServerReplaceItem(UItemObject* ItemToReplace, FIntPoint InLocation);

	UFUNCTION(Client, Reliable)
	void ClientReplaceItemRespons(UItemObject* ItemToReplace, bool bWasSuccessful);
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
	UFUNCTION(Server, Reliable)
	void ServerEquipItem(UItemObject* InItem);

	void EquipItem(UItemObject* InItem);
	/*
	* ������ ���� ���� �Լ�
	*/
	UFUNCTION(Server, Reliable)
	void ServerUnEquipItem(UItemObject* InItem);

	bool UnEquipItem(UItemObject* InItem);

	bool HandleUnEquipItem(UItemObject* InItem);
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

	void HandleSplitItem(UItemObject* InItem, int32 OriginalQuantity);

	UFUNCTION(Server, Reliable)
	void ServerSplitItem(UItemObject* InItem, int32 OriginalQuantity);


	/*
	* �κ��丮Grid�ʱ�ȭ
	*/
	UFUNCTION(BlueprintCallable, Category = "InventoryInit")
	void InitializeInventory();


	/*
	* �ش� ������ ��� Get�ϴ� �Լ�
	*/
	UFUNCTION(BlueprintCallable, Category = "Eqiupment")
	UItemObject* GetEquipItemToSlot(EEquipmentSlotType SlotType);
	
	/*
	* ��� �����Ҷ� ���Ǵ� �Լ�
	*/
	void SetEquipmentItem(UItemObject* InItem);

	/*
	* ��� �����Ҷ� ���Ǵ� �Լ�
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
	* �������� �������� InventoryGrid�� 1�� �־ ���� ä���
	*/
	void PlaceItem(UItemObject* InItem, FIntPoint InLocation);

	/*
	* �������� �����Ҷ� InventoryGrid������ ������ ������ŭ �ʱ�ȭ ��Ű��
	*/
	void RemovePlaceItem(UItemObject* InItem);

	bool CheckPlace(int32 ExpandableSize);



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

	//Replace�� �Ҷ� Item�� Ammo�ϰ�� �κ��丮�� ��� Ammoüũ�ϱ�
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
