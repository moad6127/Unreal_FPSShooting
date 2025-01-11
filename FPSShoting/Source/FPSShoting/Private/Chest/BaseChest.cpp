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

void ABaseChest::Interact(APawn* InstigatorPawn)
{
	//TODO : ����, ������ �ϱ�
	UE_LOG(LogTemp, Warning, TEXT("ChestInteract"));
	if (!HasAuthority())
	{
		ServerInteract(InstigatorPawn);
		return;
	}
	SetOwner(InstigatorPawn);
	MulticastDisplayChest(InstigatorPawn);
}

void ABaseChest::ServerInteract_Implementation(APawn* InstigatorPawn)
{
	Interact(InstigatorPawn);
}

void ABaseChest::MulticastDisplayChest_Implementation(APawn* InstigatorPawn)
{
	ASInvenFPSCharacter* Character = Cast<ASInvenFPSCharacter>(InstigatorPawn);
	if (!Character)
	{
		return;
	}
	APlayerController* PC = Cast<APlayerController>(Character->GetController());
	if (!PC)
	{
		return;
	}
	if (PC->IsLocalController())
	{
		if (ChestWidgetClass)
		{
			ChestWidget = Cast<UBasicWidget>(CreateWidget(GetWorld(), ChestWidgetClass));

			// ĳ���Ϳ� ������ �κ��丮 ������Ʈ�� ���� �Ķ���� ����
			FWidgetControllerParams CharacterWidgetControllerParams(PC, Character->GetEquipInventoryComponent());
			FWidgetControllerParams ChestWidgetControllerParams(PC, InventoryComponent);

			// ���� ��Ʈ�ѷ� ���� �� ����
			if (WidgetControllerClass)
			{
				// ĳ���� �κ��丮 ��Ʈ�ѷ� ����
				CharcterWidgetController = NewObject<UBasicWidgetController>(this, WidgetControllerClass);
				if (CharcterWidgetController)
				{
					CharcterWidgetController->SetWidgetControllerParams(CharacterWidgetControllerParams);
				}

				// ���� �κ��丮 ��Ʈ�ѷ� ����
				ChestWidgetController = NewObject<UBasicWidgetController>(this, WidgetControllerClass);
				if (ChestWidgetController)
				{
					ChestWidgetController->SetWidgetControllerParams(ChestWidgetControllerParams);
				}
			}

			DisplayChestBox.Broadcast();
		}
	}	
}

void ABaseChest::ClientDisplayChest_Implementation(APawn* InstigatorPawn)
{

	ASInvenFPSCharacter* Character = Cast<ASInvenFPSCharacter>(InstigatorPawn);
	if (!Character)
	{
		return;
	}
	APlayerController* PC = Cast<APlayerController>(Character->GetController());
	if (!PC)
	{
		return;
	}
	if (ChestWidgetClass)
	{
		ChestWidget = Cast<UBasicWidget>(CreateWidget(GetWorld(), ChestWidgetClass));

		// ĳ���Ϳ� ������ �κ��丮 ������Ʈ�� ���� �Ķ���� ����
		FWidgetControllerParams CharacterWidgetControllerParams(PC, Character->GetEquipInventoryComponent());
		FWidgetControllerParams ChestWidgetControllerParams(PC, InventoryComponent);

		// ���� ��Ʈ�ѷ� ���� �� ����
		if (WidgetControllerClass)
		{
			// ĳ���� �κ��丮 ��Ʈ�ѷ� ����
			CharcterWidgetController = NewObject<UBasicWidgetController>(this, WidgetControllerClass);
			if (CharcterWidgetController)
			{
				CharcterWidgetController->SetWidgetControllerParams(CharacterWidgetControllerParams);
			}

			// ���� �κ��丮 ��Ʈ�ѷ� ����
			ChestWidgetController = NewObject<UBasicWidgetController>(this, WidgetControllerClass);
			if (ChestWidgetController)
			{
				ChestWidgetController->SetWidgetControllerParams(ChestWidgetControllerParams);
			}
		}

		DisplayChestBox.Broadcast();

	}
}
