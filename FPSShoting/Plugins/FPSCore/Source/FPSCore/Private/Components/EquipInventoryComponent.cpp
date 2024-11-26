// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/EquipInventoryComponent.h"
#include "Items/ItemObject.h"
#include "Items/ItemBase.h"
#include "FPSCharacter.h"
#include "Components/InventoryComponent.h"
#include "FPSCharacterController.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"

UEquipInventoryComponent::UEquipInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEquipInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEquipInventoryComponent, InventoryItems);
	DOREPLIFETIME(UEquipInventoryComponent, InventoryGrid);
}

bool UEquipInventoryComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (auto& Item : InventoryItems)
	{
		bWroteSomething |= Channel->ReplicateSubobject(Item, *Bunch, *RepFlags);
	}
	return bWroteSomething;
}

bool UEquipInventoryComponent::TryAddItem(UItemObject* InItem)
{
	if (!IsValid(InItem))
	{
		return false;
	}
	for (int i = 0; i < Rows; i++)
	{
		for (int j = 0; j < Columns; j++)
		{
			// TopLeft의 위치에 아이템이 들어올수 있는지 체크하기
			FIntPoint Location(j, i);
			if (IsRoomAvailable(InItem, Location))
			{
				if (HandleAddItem(InItem))
				{
					return true;
				}
				PlaceItem(InItem, Location);
				InItem->SetItemItemLocation(Location);
				InItem->InInventorys = true;
				InItem->bIsCopy = false;
				InventoryItems.Add(InItem);
				InventoryChanged.Broadcast();
				return true;
			}
		}
	}
	return false;
}

bool UEquipInventoryComponent::AddItem(UItemObject* InItem)
{
	//위치는 정해져 있다.
	//만약 해당 위치에 다른 물건이 존재하거나 인벤토리를 넘어가면 false를 발생시킨다.
	if (IsRoomAvailable(InItem, InItem->GetItemItemLocation()))
	{
		PlaceItem(InItem, InItem->GetItemItemLocation());
		InItem->InInventorys = true;
		InItem->bIsCopy = false;
		if (InItem->SlotType == EEquipmentSlotType::EEST_Ammo)
		{
			CheckAmmo(InItem);
		}
		InventoryItems.Add(InItem);
		InventoryChanged.Broadcast();
		return true;
	}
	return false;
}


bool UEquipInventoryComponent::HandleAddItem(UItemObject* InItem)
{
	bool Flag = false;
	EEquipmentSlotType SlotType = InItem->SlotType;
	switch (SlotType)
	{
	case EEquipmentSlotType::EEST_Head:
	case EEquipmentSlotType::EEST_Chest:
	case EEquipmentSlotType::EEST_Backpack:
		/*
		* 만약 장착된 아이템이 없을경우 
		* 아이템을 장착한다.
		*/
		if (!EquipmentItems.Contains(SlotType))
		{
			EquipItem(InItem);
			Flag = true;
		}
		break;
	case EEquipmentSlotType::EEST_Ammo:
		if (!InItem->bIsCopy && !InItem->InInventorys)
		{
			AddAmmo(InItem);
		}
		break;
	case EEquipmentSlotType::EEST_Weapon:

		if (EquipmentItems.Contains(EEquipmentSlotType::EEST_Weapon1)
			&& EquipmentItems.Contains(EEquipmentSlotType::EEST_Weapon2))
		{
			break;
		}

		EquipItem(InItem);
		Flag = true;
		break;
	}
	if (InItem->ItemNumbericData.bIsStackable && !InItem->bIsCopy)
	{
		/*
		* 만약 MaxStack을 넘기지 않을경우 인벤토리창에 추가하지 않도록 만든다.
		*/
		Flag = HandleStackableItem(InItem);
	}
	return Flag;
}

bool UEquipInventoryComponent::HandleStackableItem(UItemObject* InItem)
{
	int32 InItemAmount = InItem->ItemQuantity;
	for (auto e : InventoryItems)
	{
		if (e->ID == InItem->ID)
		{
			int32 AddAmount = AddItemAmount(e, InItem->ItemQuantity);
			// 모든 개수가 기존의 아이템에 들어간다.
			if (AddAmount == InItem->ItemQuantity)
			{
				return true;
			}
			else
			{
				InItem->ItemQuantity -= AddAmount;
				continue;
			}
		}
	}
	return false;
}

void UEquipInventoryComponent::AddAmmo(UItemObject* InItem)
{
	const AFPSCharacter* PlayerCharacter = Cast<AFPSCharacter>(GetOwner());
	if (PlayerCharacter)
	{
		AFPSCharacterController* CharacterController = Cast<AFPSCharacterController>(PlayerCharacter->GetController());

		if (CharacterController)
		{
			CharacterController->AmmoMap[InItem->WeaponData.AmmoType] += InItem->ItemQuantity;
		}
	}
}

void UEquipInventoryComponent::RemoveAmmo(UItemObject* InItem)
{
	const AFPSCharacter* PlayerCharacter = Cast<AFPSCharacter>(GetOwner());
	if (PlayerCharacter)
	{
		AFPSCharacterController* CharacterController = Cast<AFPSCharacterController>(PlayerCharacter->GetController());
		if (CharacterController)
		{
			CharacterController->AmmoMap[InItem->WeaponData.AmmoType] -= InItem->ItemQuantity;
		}
	}
}

void UEquipInventoryComponent::CheckAmmo(UItemObject* InItem)
{
	if (InItem->SlotType != EEquipmentSlotType::EEST_Ammo)
	{
		return;
	}
	EAmmoType AmmoType = InItem->WeaponData.AmmoType;
	const AFPSCharacter* PlayerCharacter = Cast<AFPSCharacter>(GetOwner());
	if (PlayerCharacter)
	{
		AFPSCharacterController* CharacterController = Cast<AFPSCharacterController>(PlayerCharacter->GetController());
		if (CharacterController)
		{
			CharacterController->AmmoMap[AmmoType] = 0;
			for (auto Item : InventoryItems)
			{
				if (Item->WeaponData.AmmoType == AmmoType)
				{
					CharacterController->AmmoMap[AmmoType] += Item->ItemQuantity;
				}
			}
		}
	}

}

void UEquipInventoryComponent::OnRep_InventoryItems()
{
	InventoryChanged.Broadcast();
}

bool UEquipInventoryComponent::RemoveItems(UItemObject* InItem)
{
	if (!IsValid(InItem))
	{
		return false;
	}
	if (InventoryItems.Contains(InItem))
	{
		InventoryItems.Remove(InItem);
		RemovePlaceItem(InItem);
		//CheckAmmo(InItem);
		InventoryChanged.Broadcast();
		return true;
	}
	return false;
}

void UEquipInventoryComponent::DropItem(UItemObject* ItemToDrop)
{
	// SpawnParasm를 생성한후 설정해주기
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	//위치를 정해주기 위치는 현재 Component를 가지고 있는 Actor앞의 50정도 위치
	const FVector SpawnLocation{ GetOwner()->GetActorLocation() + (GetOwner()->GetActorForwardVector() * 50.f) };
	const FTransform SpawnTransform(GetOwner()->GetActorRotation(), SpawnLocation);

	AItemBase* DropItem = GetWorld()->SpawnActor<AItemBase>(AItemBase::StaticClass(), SpawnTransform, SpawnParams);
	//Drop아이템에 대해서 Initialize해주기
	ItemToDrop->ResetItemFlags();
	DropItem->InitializeDrop(ItemToDrop);
	CheckAmmo(ItemToDrop);
}

bool UEquipInventoryComponent::ReplaceItem(UItemObject* ItemToReplace, FIntPoint InLocation)
{
	if (!IsValid(ItemToReplace))
	{
		return false;
	}
	if (IsRoomAvailable(ItemToReplace, InLocation))
	{
		PlaceItem(ItemToReplace, InLocation);
		ItemToReplace->SetItemItemLocation(InLocation);
		InventoryItems.Add(ItemToReplace);
		if (ItemToReplace->SlotType == EEquipmentSlotType::EEST_Ammo)
		{
			CheckAmmo(ItemToReplace);
		}
		InventoryChanged.Broadcast();
		return true;
	}
	return false;
}

void UEquipInventoryComponent::RotateItem(UItemObject* ItemToRotate)
{
	ItemToRotate->Rotate();
	InventoryChanged.Broadcast();
}

bool UEquipInventoryComponent::IsRoomAvailable(UItemObject* InItem, FIntPoint InLocation)
{
	// BottomRight가 인벤토리 범위를 넘어가는지 확인하기

	int32 BottomRightX = InLocation.X + InItem->GetSizeX();
	int32 BottomRightY = InLocation.Y + InItem->GetSizeY();
	FIntPoint BottomRight(BottomRightX, BottomRightY);
	if (!IsPositionValid(InLocation))
	{
		return false;
	}
	if (!IsPositionValid(BottomRight))
	{
		return false;
	}
	// TopLeft부터 아이템의 사이즈 까지 인벤토리 공간 체크
	// EX) TopLeft가 1,1이고 아이템 Size가 3x3이면
	// BottomRight는 1 + 3의 값을 가진다.
	for (int32 i = InLocation.Y; i < InLocation.Y + InItem->GetSizeY(); i++)
	{
		for (int32 j = InLocation.X; j < InLocation.X + InItem->GetSizeX(); j++)
		{
			//인벤토리를 순회하면서 TopLeft부터 BottomRight까지 인벤토리Grid를 확인해서
			// Grid가 1일경우 false반환하기
			if (InventoryGrid[GetIndex(j, i)])
			{
				return false;
			}
		}
	}

	return true;
}

void UEquipInventoryComponent::EquipItem(UItemObject* InItem)
{

	HandleEquip(InItem);

	EEquipmentSlotType ItemSlot = InItem->SlotType;
	/*
	* 만약 이미 장착된 아이템이 존재하면 
	* 기존의 장착된 아이템을 뺀후 현재 아이템을 장착한다.
	*/

	//Backpack && Chest
	if (InItem->ItemNumbericData.bExpandableSize)
	{
		Rows += InItem->ItemNumbericData.ExpandableInventorySize;
		InventorySizeChanged.Broadcast();
	}
}

void UEquipInventoryComponent::HandleEquip(UItemObject* InItem)
{
	EEquipmentSlotType ItemSlot = InItem->SlotType;
	AFPSCharacter* FPSCharacter = Cast<AFPSCharacter>(GetOwner());
	if (FPSCharacter == nullptr)
	{
		return;
	}
	switch (ItemSlot)
	{
	case EEquipmentSlotType::EEST_Head:
		if (EquipmentItems.Contains(ItemSlot))
		{
			UItemObject* PrevItem = EquipmentItems[ItemSlot];
			UnEquipItem(PrevItem);
		}
		break;
	case EEquipmentSlotType::EEST_Chest:
		if (EquipmentItems.Contains(ItemSlot))
		{
			UItemObject* PrevItem = EquipmentItems[ItemSlot];
			UnEquipItem(PrevItem);
		}
		break;
	case EEquipmentSlotType::EEST_Backpack:
		if (EquipmentItems.Contains(ItemSlot))
		{
			UItemObject* PrevItem = EquipmentItems[ItemSlot];
			UnEquipItem(PrevItem);
		}
		break;
	case EEquipmentSlotType::EEST_Weapon:
		// Weapon1에 무기가 장착되어있으면
	{
		int InventorySlot{};
		bool bSwapingWeapon{};
		if (EquipmentItems.Contains(EEquipmentSlotType::EEST_Weapon1))
		{
			//Weapon2에도 무기가 장착되어 있으면
			if (EquipmentItems.Contains(EEquipmentSlotType::EEST_Weapon2))
			{
				//현재 무기의 index를 구해서 그곳으로slot을 정한다.
				// Index가 0일경우 weapon1 1일경우 weapon2로 만든다.
				bSwapingWeapon = true;
				if (InventorySlot == 0)
				{
					ItemSlot = EEquipmentSlotType::EEST_Weapon1;
				}
				else
				{
					ItemSlot = EEquipmentSlotType::EEST_Weapon2;
				}
			}
			else
			{
				//weapon1에 장착되어 있지만 2에는 장착이 안됨
				ItemSlot = EEquipmentSlotType::EEST_Weapon2;
				InventorySlot = 1;
			}
		}
		else
		{
			//아무것도 장착되어 있지 않을때
			ItemSlot = EEquipmentSlotType::EEST_Weapon1;
		}

		if (bSwapingWeapon)
		{
			TryAddItem(InItem);
		}
		else
		{
			// UPDATEITEM
			InItem->SlotType = ItemSlot;
			const FVector SpawnLocation{ GetOwner()->GetActorLocation() + (GetOwner()->GetActorForwardVector() * 50.f) };
			const FTransform SpawnTransform(GetOwner()->GetActorRotation(), SpawnLocation);

			FPSCharacter->GetInventoryComponent()->UpdateWeapon(
				InItem->WeaponData.WeaponReference,
				InventorySlot,
				bSwapingWeapon,
				true,
				SpawnTransform,
				InItem->DataStruct);
		}

	}
		break;
	default:
		break;
	}
	EquipmentItems.Add({ ItemSlot,InItem });
	ItemEquipChanged.Broadcast(ItemSlot);
}




bool UEquipInventoryComponent::UnEquipItem(UItemObject* InItem)
{
	EEquipmentSlotType SlotType = InItem->SlotType;
	if (!EquipmentItems.Contains(SlotType))
	{
		UE_LOG(LogTemp, Warning, TEXT("Not have item in EquipmentItems!!"));
		return false;
	}

	//만약 증가된 인벤토리에 아이템이 있으면 장비해제를 못하게 하기
	if (InItem->ItemNumbericData.bExpandableSize)
	{
		if (!CheckPlace(InItem->ItemNumbericData.ExpandableInventorySize))
		{
			return false;
		}
	}
	// Equip창에 Slot을 구분하기위해 Weapon은 1과 2로 나누었다.

	if (SlotType == EEquipmentSlotType::EEST_Weapon1 || SlotType == EEquipmentSlotType::EEST_Weapon2)
	{
		int index{};
		if (SlotType == EEquipmentSlotType::EEST_Weapon2)
		{
			index = 1;
		}
		if (AFPSCharacter* FPSCharacter = Cast<AFPSCharacter>(GetOwner()))
		{
			InItem->DataStruct = *FPSCharacter->GetInventoryComponent()->GetWeaponByID(index)->GetRuntimeWeaponData();
			FPSCharacter->GetInventoryComponent()->RemoveEquipItems(index);
			
		}
		InItem->SlotType = EEquipmentSlotType::EEST_Weapon;
		UE_LOG(LogTemp, Warning, TEXT("NowAmmo : %d"), InItem->DataStruct.ClipSize);
	}
	// 제거하는것은 들어온것의 SlotType을 제거한다


	EquipmentItems.Remove(SlotType);
	ItemEquipChanged.Broadcast(SlotType);
	if (InItem->ItemNumbericData.bExpandableSize)
	{
		Rows -= InItem->ItemNumbericData.ExpandableInventorySize;
		InventorySizeChanged.Broadcast();
	}
	return true;
}

void UEquipInventoryComponent::ConsumeItem(UItemObject* InItem, int32 ConsumeAmount)
{
	if (InItem->ItemNumbericData.bIsStackable)
	{
		int32 RemoveAmount = FMath::Min(ConsumeAmount, InItem->ItemQuantity);
		InItem->ItemQuantity -= RemoveAmount;
		InventoryChanged.Broadcast();
	}
}

int32 UEquipInventoryComponent::AddItemAmount(UItemObject* InItem, int32 AddAmount)
{	
	int32 NeedMaxQuantity = InItem->ItemNumbericData.MaxStackSize - InItem->ItemQuantity;
	if (NeedMaxQuantity >= AddAmount)
	{
		InItem->ItemQuantity += AddAmount;
		return AddAmount;
	}
	else
	{
		InItem->ItemQuantity = InItem->ItemNumbericData.MaxStackSize;
		return NeedMaxQuantity;
	}
	return 0;
}

void UEquipInventoryComponent::SplitItem(UItemObject* InItem)
{
	if (InItem->ItemNumbericData.bIsStackable)
	{
		int32 SplitAmount = InItem->ItemQuantity / 2;
		if (SplitAmount)
		{
			UItemObject* NewItem;
			NewItem = InItem->CreateItemCopy();

			NewItem->ItemQuantity = SplitAmount;
			ConsumeItem(InItem, SplitAmount);


			if (!TryAddItem(NewItem))
			{
				DropItem(NewItem);
			}
		}
	}

}


void UEquipInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializeInventory();
	StarterItem();
}

void UEquipInventoryComponent::StarterItem()
{
	for (auto StartItem : StartItems)
	{
		if (StartItem.ItemDataTable && !StartItem.ItemName.IsNone())
		{
			const FItemData* ItemData = StartItem.ItemDataTable->FindRow<FItemData>(StartItem.ItemName, StartItem.ItemName.ToString());

			UItemObject* ItemObject = NewObject<UItemObject>(this, UItemObject::StaticClass());

			ItemObject->ID = ItemData->ID;
			ItemObject->SlotType = ItemData->SlotType;
			ItemObject->ItemQuantity = StartItem.ItemQuantity;
			ItemObject->ItemNumbericData = ItemData->ItemNumbericData;
			ItemObject->Asset = ItemData->Asset;
			ItemObject->ItemName = ItemData->ItemName;
			ItemObject->WeaponData = ItemData->WeaponData;
			ItemObject->SetItemSizeX(ItemData->SizeX);
			ItemObject->SetItemSizeY(ItemData->SizeY);

			if (!TryAddItem(ItemObject))
			{
				UE_LOG(LogTemp, Warning, TEXT("Can't Add Item : %s"), *ItemObject->ID.ToString());
			}
		}

	}
}

void UEquipInventoryComponent::PlaceItem(UItemObject* InItem, FIntPoint InLocation)
{
	for (int32 i = InLocation.Y; i < InLocation.Y + InItem->GetSizeY(); i++)
	{
		for (int32 j = InLocation.X; j < InLocation.X + InItem->GetSizeX(); j++)
		{
			InventoryGrid[GetIndex(j, i)] = true;
		}
	}
}

void UEquipInventoryComponent::RemovePlaceItem(UItemObject* InItem)
{
	FIntPoint InLocation = InItem->GetItemItemLocation();
	for (int32 i = InLocation.Y; i < InLocation.Y + InItem->GetSizeY(); i++)
	{
		for (int32 j = InLocation.X; j < InLocation.X + InItem->GetSizeX(); j++)
		{
			InventoryGrid[GetIndex(j, i)] = false;
		}
	}
}

bool UEquipInventoryComponent::CheckPlace(int32 ExpandableSize)
{
	for (int i = Rows - ExpandableSize; i < Rows; i++)
	{
		for (int j = 0; j < Columns; j++)
		{
			if (InventoryGrid[GetIndex(j, i)])
			{
				// 추가된 인벤토리에 아이템이 존재하면 false를 반환
				return false;
			}
		}
	}
	return true;
}


void UEquipInventoryComponent::InitializeInventory()
{
	if (InventoryGrid.IsEmpty())
	{
		InventoryGrid.Init(false, 100 * 100);
	}
}

bool UEquipInventoryComponent::IsPositionValid(FIntPoint InLocation)
{
	// 아이템을 넣을때 해당 위치가 올바른지 확인
	return InLocation.X >= 0 && InLocation.X <= Columns && InLocation.Y >= 0 && InLocation.Y <= Rows;
}


