// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/EquipInventoryComponent.h"
#include "Items/ItemObject.h"

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

void UEquipInventoryComponent::InitializeInventory()
{
	InventoryGrid.Init(false, Columns * Rows);
}

bool UEquipInventoryComponent::IsPositionValid(FIntPoint InLocation)
{
	// �������� ������ �ش� ��ġ�� �ùٸ��� Ȯ��
	return InLocation.X >= 0 && InLocation.X <= Columns && InLocation.Y >= 0 && InLocation.Y <= Rows;
}

