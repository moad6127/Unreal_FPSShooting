// Fill out your copyright notice in the Description page of Project Settings.


#include "Chest/BaseChest.h"
#include "Components/EquipInventoryComponent.h"
#include "Widget/BasicWidgetController.h"
#include "Widget/BasicWidget.h"
#include "Character/SInvenFPSCharacter.h"
#include "HUD/FPSHUD.h"


ABaseChest::ABaseChest()
{
	bReplicates = true;
	InteractionText = FText::FromName(FName("Chest"));
	InventoryComponent = CreateDefaultSubobject<UEquipInventoryComponent>(TEXT("InventoryComp"));
	InventoryComponent->SetIsReplicated(true);
}

void ABaseChest::Interact()
{
	if (!ChestWidgetClass)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("ChestInteract"));

	//TODO : 사운드, 문열림 하기
	// BlueprintImplementableEvent로 Interact하게되면 블루프린트로 화면에 보이도록 만들기
	ASInvenFPSCharacter* Character = Cast<ASInvenFPSCharacter>(GetOwner());
	if (!Character)
	{
		return;	
	}
	if (!Character->IsLocallyControlled())
	{
		return;
	}
	APlayerController* PC = Cast<APlayerController>(Character->GetController());
	if (!PC)
	{
		return;
	}
	ChestWidget = Cast<UBasicWidget>(CreateWidget(GetWorld(), ChestWidgetClass));
	FWidgetControllerParams CharcterWidgetControllerParams(PC, Character->GetEquipInventoryComponent());
	FWidgetControllerParams ChestWidgetControllerParams(PC, InventoryComponent);

	if (WidgetControllerClass == nullptr)
	{
		return;
	}
	CharcterWidgetController = NewObject<UBasicWidgetController>(this, WidgetControllerClass);
	if (!CharcterWidgetController)
	{
		return;
	}
	CharcterWidgetController->SetWidgetControllerParams(CharcterWidgetControllerParams);


	ChestWidgetController = NewObject<UBasicWidgetController>(this, WidgetControllerClass);
	if (!ChestWidgetController)
	{
		return;
	}
	ChestWidgetController->SetWidgetControllerParams(ChestWidgetControllerParams);

	DisplayChestBox.Broadcast();
	//DisplayChest();
}
