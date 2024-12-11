// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemBase.h"
#include "Items/ItemObject.h"
#include "Components/SphereComponent.h"
#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

#include "Components/EquipInventoryComponent.h"

AItemBase::AItemBase()
{
	bAlwaysRelevant = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("ItemCollision");
	Sphere->SetupAttachment(MeshComp);	
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
}

void AItemBase::Interact()
{
	const AFPSCharacter* PlayerCharacter = Cast<AFPSCharacter>(GetOwner());
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
	InitializeItemObject(UItemObject::StaticClass());
	Init();
}

void AItemBase::OnRep_ItemObject()
{
	MeshComp->SetStaticMesh(ItemObject->Asset.Mesh);
	InteractionText = ItemObject->ItemName;
}


void AItemBase::InitializeItemObject(const TSubclassOf<UItemObject> BaseItem)
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
		ItemObject->SetItemSizeX(ItemData->SizeX);
		ItemObject->SetItemSizeY(ItemData->SizeY);

		if (ItemData->Asset.Mesh)
		{
			MeshComp->SetStaticMesh(ItemData->Asset.Mesh);
		}
		InteractionText = ItemObject->ItemName;
	}
}

void AItemBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItemBase, ItemObject);
}

void AItemBase::Init()
{
	if (ItemObject && ItemObject->WeaponData.bIsWeapon && ItemObject->WeaponData.WeaponReference)
	{
		WeaponReference = ItemObject->WeaponData.WeaponReference;
		WeaponDataTable = ItemObject->WeaponData.WeaponDataTable;
		AttachmentsDataTable = ItemObject->WeaponData.AttachmentsDataTable;
		AttachmentArrayOverride = ItemObject->WeaponData.AttachmentArrayOverride;

		const AWeaponBase* WeaponBaseReference = WeaponReference.GetDefaultObject();
		if (const FStaticWeaponData* StaticWeaponData = WeaponDataTable->FindRow<FStaticWeaponData>(FName(WeaponBaseReference->GetDataTableNameRef()), FString(WeaponBaseReference->GetDataTableNameRef()), true))
		{
			if (!bRuntimeSpawned)
			{
				DataStruct.AmmoType = StaticWeaponData->AmmoToUse;
				DataStruct.ClipCapacity = StaticWeaponData->ClipCapacity;
				DataStruct.ClipSize = StaticWeaponData->ClipSize;
				DataStruct.WeaponAttachments = AttachmentArrayOverride;
				DataStruct.WeaponHealth = 100.0f;
			}
		}
		ItemObject->DataStruct = DataStruct;
	}
}

