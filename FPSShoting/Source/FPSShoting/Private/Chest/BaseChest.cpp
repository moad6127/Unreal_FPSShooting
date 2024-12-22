// Fill out your copyright notice in the Description page of Project Settings.


#include "Chest/BaseChest.h"
#include "Components/EquipInventoryComponent.h"
#include "Widget/BasicWidgetController.h"


ABaseChest::ABaseChest()
{
	bReplicates = true;
	InteractionText = FText::FromName(FName("Chest"));
	InventoryComponent = CreateDefaultSubobject<UEquipInventoryComponent>(TEXT("InventoryComp"));
	InventoryComponent->SetIsReplicated(true);
}

void ABaseChest::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("ChestInteract"));
	//TODO : ����, ������ �ϱ�
	// BlueprintImplementableEvent�� Interact�ϰԵǸ� �������Ʈ�� ȭ�鿡 ���̵��� �����

	DisplayChestBox.Broadcast();
	//DisplayChest();
}
