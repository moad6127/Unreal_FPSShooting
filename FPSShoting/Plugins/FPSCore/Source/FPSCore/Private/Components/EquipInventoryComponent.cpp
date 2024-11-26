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
			// TopLeft�� ��ġ�� �������� ���ü� �ִ��� üũ�ϱ�
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
	//��ġ�� ������ �ִ�.
	//���� �ش� ��ġ�� �ٸ� ������ �����ϰų� �κ��丮�� �Ѿ�� false�� �߻���Ų��.
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
		* ���� ������ �������� ������� 
		* �������� �����Ѵ�.
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
		* ���� MaxStack�� �ѱ��� ������� �κ��丮â�� �߰����� �ʵ��� �����.
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
			// ��� ������ ������ �����ۿ� ����.
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
	// SpawnParasm�� �������� �������ֱ�
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	//��ġ�� �����ֱ� ��ġ�� ���� Component�� ������ �ִ� Actor���� 50���� ��ġ
	const FVector SpawnLocation{ GetOwner()->GetActorLocation() + (GetOwner()->GetActorForwardVector() * 50.f) };
	const FTransform SpawnTransform(GetOwner()->GetActorRotation(), SpawnLocation);

	AItemBase* DropItem = GetWorld()->SpawnActor<AItemBase>(AItemBase::StaticClass(), SpawnTransform, SpawnParams);
	//Drop�����ۿ� ���ؼ� Initialize���ֱ�
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
	// BottomRight�� �κ��丮 ������ �Ѿ���� Ȯ���ϱ�

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
	// TopLeft���� �������� ������ ���� �κ��丮 ���� üũ
	// EX) TopLeft�� 1,1�̰� ������ Size�� 3x3�̸�
	// BottomRight�� 1 + 3�� ���� ������.
	for (int32 i = InLocation.Y; i < InLocation.Y + InItem->GetSizeY(); i++)
	{
		for (int32 j = InLocation.X; j < InLocation.X + InItem->GetSizeX(); j++)
		{
			//�κ��丮�� ��ȸ�ϸ鼭 TopLeft���� BottomRight���� �κ��丮Grid�� Ȯ���ؼ�
			// Grid�� 1�ϰ�� false��ȯ�ϱ�
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
	* ���� �̹� ������ �������� �����ϸ� 
	* ������ ������ �������� ���� ���� �������� �����Ѵ�.
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
		// Weapon1�� ���Ⱑ �����Ǿ�������
	{
		int InventorySlot{};
		bool bSwapingWeapon{};
		if (EquipmentItems.Contains(EEquipmentSlotType::EEST_Weapon1))
		{
			//Weapon2���� ���Ⱑ �����Ǿ� ������
			if (EquipmentItems.Contains(EEquipmentSlotType::EEST_Weapon2))
			{
				//���� ������ index�� ���ؼ� �װ�����slot�� ���Ѵ�.
				// Index�� 0�ϰ�� weapon1 1�ϰ�� weapon2�� �����.
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
				//weapon1�� �����Ǿ� ������ 2���� ������ �ȵ�
				ItemSlot = EEquipmentSlotType::EEST_Weapon2;
				InventorySlot = 1;
			}
		}
		else
		{
			//�ƹ��͵� �����Ǿ� ���� ������
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

	//���� ������ �κ��丮�� �������� ������ ��������� ���ϰ� �ϱ�
	if (InItem->ItemNumbericData.bExpandableSize)
	{
		if (!CheckPlace(InItem->ItemNumbericData.ExpandableInventorySize))
		{
			return false;
		}
	}
	// Equipâ�� Slot�� �����ϱ����� Weapon�� 1�� 2�� ��������.

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
	// �����ϴ°��� ���°��� SlotType�� �����Ѵ�


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
				// �߰��� �κ��丮�� �������� �����ϸ� false�� ��ȯ
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
	// �������� ������ �ش� ��ġ�� �ùٸ��� Ȯ��
	return InLocation.X >= 0 && InLocation.X <= Columns && InLocation.Y >= 0 && InLocation.Y <= Rows;
}


