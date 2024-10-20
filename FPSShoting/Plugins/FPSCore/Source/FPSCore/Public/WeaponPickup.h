// Copyright 2022 Ellie Kelemen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InteractionActor.h"
#include "Items/ItemBase.h"
#include "WeaponPickup.generated.h"

class UStaticMeshComponent;
class UDataTable;
class AWeaponBase;
class AFPSCharacter;

UCLASS()
class FPSCORE_API AWeaponPickup : public AItemBase
{
	GENERATED_BODY()


public:

	/** Returns the main mesh component of the given pickup - useful for toggling setting such as physics simulation */
	UStaticMeshComponent* GetMainMesh() const { return MeshComp; }

	/** Set the static setting of the given pickup (whether it should simulate physics or not */
	void SetStatic(const bool bNewStatic) { bStatic = bNewStatic; }


	/** Set the weapon class that this pickup corresponds to */
	void SetWeaponReference(const TSubclassOf<AWeaponBase> NewWeaponReference) { WeaponReference = NewWeaponReference; }

	/** Set the cached data struct that this weapon pickup hold */
	void SetCacheDataStruct(const FRuntimeWeaponData* NewDataStruct) { DataStruct = *NewDataStruct; }

	/** Returns the name of the weapon that this pickup is associated with (used for HUD) */
	FText GetWeaponName() const { return WeaponName; } 
	
	/** Spawns attachment meshes from data table */
	UFUNCTION(BlueprintCallable, Category = "Weapon Pickup")
	void SpawnAttachmentMesh();
	


private:	
	/** Sets default values for this actor's properties */
	AWeaponPickup();

	/** Interface function */
	virtual void Interact() override;

	/** Called every time a variable is changed or the actor is moved in the editor */
	virtual void OnConstruction(const FTransform& Transform) override;





	/** Whether to run physics simulations or not */
	UPROPERTY(EditInstanceOnly, Category = "Weapon")
	bool bStatic;

	/** Whether this weapon will become the primary or secondary weapon */
	bool bIsNewPrimary;
	
	/** Visualisation name */
	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	FText WeaponName;

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Meshes for Attachments */



	UPROPERTY()
	UStaticMeshComponent* BarrelAttachment;

	UPROPERTY()
	UStaticMeshComponent* MagazineAttachment;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* SightsAttachment;
	
	UPROPERTY()
	UStaticMeshComponent* StockAttachment;

	UPROPERTY()
	UStaticMeshComponent* GripAttachment;
};
