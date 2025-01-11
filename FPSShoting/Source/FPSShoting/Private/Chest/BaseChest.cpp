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
	//TODO : 사운드, 문열림 하기
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

			// 캐릭터와 상자의 인벤토리 컴포넌트를 위한 파라미터 설정
			FWidgetControllerParams CharacterWidgetControllerParams(PC, Character->GetEquipInventoryComponent());
			FWidgetControllerParams ChestWidgetControllerParams(PC, InventoryComponent);

			// 위젯 컨트롤러 생성 및 설정
			if (WidgetControllerClass)
			{
				// 캐릭터 인벤토리 컨트롤러 설정
				CharcterWidgetController = NewObject<UBasicWidgetController>(this, WidgetControllerClass);
				if (CharcterWidgetController)
				{
					CharcterWidgetController->SetWidgetControllerParams(CharacterWidgetControllerParams);
				}

				// 상자 인벤토리 컨트롤러 설정
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

		// 캐릭터와 상자의 인벤토리 컴포넌트를 위한 파라미터 설정
		FWidgetControllerParams CharacterWidgetControllerParams(PC, Character->GetEquipInventoryComponent());
		FWidgetControllerParams ChestWidgetControllerParams(PC, InventoryComponent);

		// 위젯 컨트롤러 생성 및 설정
		if (WidgetControllerClass)
		{
			// 캐릭터 인벤토리 컨트롤러 설정
			CharcterWidgetController = NewObject<UBasicWidgetController>(this, WidgetControllerClass);
			if (CharcterWidgetController)
			{
				CharcterWidgetController->SetWidgetControllerParams(CharacterWidgetControllerParams);
			}

			// 상자 인벤토리 컨트롤러 설정
			ChestWidgetController = NewObject<UBasicWidgetController>(this, WidgetControllerClass);
			if (ChestWidgetController)
			{
				ChestWidgetController->SetWidgetControllerParams(ChestWidgetControllerParams);
			}
		}

		DisplayChestBox.Broadcast();

	}
}
