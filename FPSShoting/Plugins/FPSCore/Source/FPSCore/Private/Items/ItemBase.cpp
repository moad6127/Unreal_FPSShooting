// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemBase.h"
#include "Items/ItemObject.h"
#include "Components/SphereComponent.h"
#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/EquipInventoryComponent.h"

AItemBase::AItemBase()
{
	Sphere = CreateDefaultSubobject<USphereComponent>("ItemCollision");
	Sphere->SetupAttachment(MeshComp);	
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
}

void AItemBase::Interact()
{
	const AFPSCharacter* PlayerCharacter = Cast<AFPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerCharacter->GetEquipInventoryComponent())
	{
		if (PlayerCharacter->GetEquipInventoryComponent()->TryAddItem(ItemObject))
		{
			Destroy();
		}
	}
}

void AItemBase::HandleInteract()
{
}

void AItemBase::InitializeDrop(UItemObject* ItemToDrop)
{
	ItemObject = ItemToDrop;
	MeshComp->SetStaticMesh(ItemToDrop->Asset.Mesh);
	InteractionText = ItemObject->ItemName;
}

void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	InitializeItem(UItemObject::StaticClass());
}


void AItemBase::InitializeItem(const TSubclassOf<UItemObject> BaseItem)
{
	if (ItemDataTable && !DesiredItemID.IsNone())
	{
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());
		ItemObject = NewObject<UItemObject>(this, BaseItem);
		//TODO : ItemValueSet
		ItemObject->ID = ItemData->ID;
		ItemObject->SlotType = ItemData->SlotType;
		ItemObject->ItemQuantity = ItemQuantity;
		ItemObject->ItemNumbericData = ItemData->ItemNumbericData;
		ItemObject->Asset = ItemData->Asset;
		ItemObject->ItemName = ItemData->ItemName;
		ItemObject->WeaponData = ItemData->WeaponData;
		ItemObject->DataStruct = DataStruct;
		ItemObject->SetItemSizeX(ItemSizeX);
		ItemObject->SetItemSizeY(ItemSizeY);

		if (ItemData->Asset.Mesh)
		{
			MeshComp->SetStaticMesh(ItemData->Asset.Mesh);
		}
		InteractionText = ItemObject->ItemName;
	}
	else
	{
		return;
	}
}
