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
			// TopLeft의 위치에 아이템이 들어올수 있는지 체크하기
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
	// 아이템을 넣을때 해당 위치가 올바른지 확인
	return InLocation.X >= 0 && InLocation.X <= Columns && InLocation.Y >= 0 && InLocation.Y <= Rows;
}

