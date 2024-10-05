// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SInvenFPSCharacter.h"
#include "HUD/FPSHUD.h"
#include "Components/EquipInventoryComponent.h"

ASInvenFPSCharacter::ASInvenFPSCharacter()
{
	SInventoryComponent = CreateDefaultSubobject<UEquipInventoryComponent>("EquipInventory");
}

void ASInvenFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		HUD = Cast<AFPSHUD>(PlayerController->GetHUD());
		if (HUD)
		{
			HUD->InitHUD(PlayerController, SInventoryComponent);
		}
	}
}

void ASInvenFPSCharacter::ShowInventory()
{
	if (HUD)
	{
		HUD->ShowInventory();
	}
}
