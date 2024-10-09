// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/EquipInventoryComponent.h"
#include "Items/ItemObject.h"
#include "Items/ItemBase.h"

UEquipInventoryComponent::UEquipInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
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
				PlaceItem(InItem, Location);
				InItem->SetItemItemLocation(Location);
				InventoryItems.Add(InItem);
				InventoryChanged.Broadcast();
				return true;
			}
		}
	}
	return false;
}

bool UEquipInventoryComponent::RemoveItems(UItemObject* InItem)
{
	if (!IsValid(InItem))
	{
		return false;
	}
	if (InventoryItems.Contains(InItem))
	{
		//TODO : ���� Stackable�������̸� ������ ������ -1�ϱ�
		InventoryItems.Remove(InItem);
		RemovePlaceItem(InItem);
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
	DropItem->InitializeDrop(ItemToDrop);
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
	EEquipmentSlotType ItemSlot = InItem->SlotType;
	/*
	* ���� �̹� ������ �������� �����ϸ� 
	* ������ ������ �������� ���� ���� �������� �����Ѵ�.
	*/
	if (EquipmentItems.Contains(ItemSlot))
	{
		UItemObject* PrevItem = EquipmentItems[ItemSlot];
		UnEquipItem(PrevItem);
	}
	EquipmentItems.Add({ ItemSlot,InItem });
	if (InItem->ItemNumbericData.bExpandableSize)
	{
		Rows += InItem->ItemNumbericData.ExpandableInventorySize;
		InventorySizeChanged.Broadcast();
	}
}

void UEquipInventoryComponent::UnEquipItem(UItemObject* InItem)
{
	EEquipmentSlotType SlotType = InItem->SlotType;
	if (!EquipmentItems.Contains(SlotType))
	{
		UE_LOG(LogTemp, Warning, TEXT("Not have item in EquipmentItems!!"));
		return;
	}
	// Equipâ�� Slot�� �����ϱ����� Weapon�� 1�� 2�� ��������.
	if (SlotType == EEquipmentSlotType::EEST_Weapon1 || SlotType == EEquipmentSlotType::EEST_Weapon2)
	{
		InItem->SlotType = EEquipmentSlotType::EEST_Weapon;
	}
	// �����ϴ°��� ���°��� SlotType�� �����Ѵ�
	EquipmentItems.Remove(SlotType);
	if (InItem->ItemNumbericData.bExpandableSize)
	{
		Rows -= InItem->ItemNumbericData.ExpandableInventorySize;
		InventorySizeChanged.Broadcast();
	}
}

void UEquipInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializeInventory();
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

void UEquipInventoryComponent::InitializeInventory()
{
	InventoryGrid.Init(false, Columns * Rows);
}

bool UEquipInventoryComponent::IsPositionValid(FIntPoint InLocation)
{
	// �������� ������ �ش� ��ġ�� �ùٸ��� Ȯ��
	return InLocation.X >= 0 && InLocation.X <= Columns && InLocation.Y >= 0 && InLocation.Y <= Rows;
}

