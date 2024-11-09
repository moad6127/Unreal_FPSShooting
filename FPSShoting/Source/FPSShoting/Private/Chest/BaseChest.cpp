// Fill out your copyright notice in the Description page of Project Settings.


#include "Chest/BaseChest.h"

ABaseChest::ABaseChest()
{
	InteractionText = FText::FromName(FName("Chest"));
}

void ABaseChest::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("ChestInteract"));
	//TODO : 사운드, 문열림 하기
	// BlueprintImplementableEvent로 Interact하게되면 블루프린트로 화면에 보이도록 만들기

	DisplayChest();
}
