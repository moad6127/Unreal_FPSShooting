// Fill out your copyright notice in the Description page of Project Settings.


#include "Chest/BaseChest.h"

ABaseChest::ABaseChest()
{
	InteractionText = FText::FromName(FName("Chest"));
}

void ABaseChest::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("ChestInteract"));
	//TODO : ����, ������ �ϱ�
	// BlueprintImplementableEvent�� Interact�ϰԵǸ� �������Ʈ�� ȭ�鿡 ���̵��� �����

	DisplayChest();
}
