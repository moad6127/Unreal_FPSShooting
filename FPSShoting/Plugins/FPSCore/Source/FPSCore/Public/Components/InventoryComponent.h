// Copyright 2022 Ellie Kelemen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "WeaponBase.h"
#include "FPSCharacter.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UCameraComponent;
class UInventoryComponent;
class UItemObject;

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FHitActor, UInventoryComponent, EventHitActor, FHitResult, HitResult);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FFailedToReload, UInventoryComponent, EventFailedToReload);

UENUM(BlueprintType)
enum class EReloadFailedBehaviour : uint8
{
	Retry			UMETA(DisplayName = "Retry until successful"),
	ChangeState		UMETA(DisplayName = "Change movement state to be able to successfuly reload"),
	HandleInBP		UMETA(DisplayName = "Handle in Blueprint"),
	Ignore			UMETA(DisplayName = "Ignore unsuccessful reload")
};

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unoccupied	UMETA(DisplayName = "Unoccupied"),
	ECS_Reloading	UMETA(DisplayName = "Reloading"),
	ECS_Swaping 	UMETA(DisplayName = "Swaping"),

	ECS_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EWeaponSwapBehaviour : uint8
{
	UseNewValue		UMETA(DisplayName = "Swap to new value"),
	Ignore			UMETA(DisplayName = "Ignore subsequent swaps")
};

USTRUCT()
struct FStarterWeaponData
{
	UPROPERTY(EditDefaultsOnly, Category = "Data Table")
	TSubclassOf<AWeaponBase> WeaponClassRef;

	UPROPERTY(EditDefaultsOnly, Category = "Data Table")
	UDataTable* WeaponDataTableRef;

	/** Data table reference for attachments */
	UPROPERTY(EditDefaultsOnly, Category = "Data Table")
	UDataTable* AttachmentsDataTable;

	/** Local weapon data struct to keep track of ammo amounts and weapon health */
	UPROPERTY()
	FRuntimeWeaponData DataStruct;

	/** The array of attachments to spawn (usually inherited, can be set by instance) */
	UPROPERTY(EditDefaultsOnly, Category = "Data Table")
	TArray<FName> AttachmentArrayOverrideRef;
	
	GENERATED_BODY()
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSCORE_API UInventoryComponent final : public UActorComponent
{
	GENERATED_BODY()

public:	
	/** Sets default values for this component's properties */
	UInventoryComponent();

	/** Called to bind functionality to input */
	void SetupInputComponent(class UEnhancedInputComponent* PlayerInputComponent);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Equipping a new weapon
	 * @param NewWeapon The new weapon which to spawn
	 * @param InventoryPosition The position in the player's inventory in which to place the weapon
	 * @param bSpawnPickup Whether to spawn a pickup of CurrentWeapon (can be false if player has an empty weapon slot)
	 * @param bStatic Whether the spawned pickup should be static or run a physics simulation
	 * @param PickupTransform The position at which to spawn the new pickup, in the case that it is static (bStatic)
	 * @param DataStruct The FRuntimeWeaponData struct for the newly equipped weapon
	 */
	//void UpdateWeapon(TSubclassOf<AWeaponBase> NewWeapon, int InventoryPosition, bool bSpawnPickup, bool bStatic, FTransform PickupTransform, FRuntimeWeaponData DataStruct);
	void EquipWeapon(UItemObject* ItemObject, int InventoryPosition);

	void DropWeapon(FActorSpawnParameters& SpawnParameters, const bool& bStatic, const FTransform& PickupTransform, const int& InventoryPosition);

	UFUNCTION(Server, Reliable)
	void ServerRemoveEquipItems(int index);

	void RemoveEquipItems(int index);

	void HandleRemoveEquipItems(int index);

	/** Returns the number of weapon slots */
	UFUNCTION(BlueprintCallable, Category = "Inventory Component")
	int GetNumberOfWeaponSlots() const { return NumberOfWeaponSlots; }

	/** Returns the currently equipped weapon slot */
	UFUNCTION(BlueprintCallable, Category = "Inventory Component")
	int GetCurrentWeaponSlot() const { return CurrentWeaponSlot; }

	UFUNCTION(BlueprintCallable)
	void ReloadFinish();

	UFUNCTION(BlueprintCallable)
	ECharacterState GetCharcterState();

	void SetAmmo(EAmmoType AmmoType, int32 Amount);
	void AddAmmo(EAmmoType AmmoType, int32 Amount);

	int32 GetAmmo(EAmmoType AmmoType);

	UFUNCTION(BlueprintCallable)
	bool GetUseFABRIK();

	/** Returns the map of currently equipped weapons */
	//UFUNCTION(BlueprintCallable, Category = "Inventory Component")
	//TMap<int, AWeaponBase*> GetEquippedWeapons() const { return EquippedWeapons; }
	
	/** Returns an equipped weapon
	 *	@param WeaponID The ID of the weapon to get
	 *	@return The weapon with the given ID
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory Component")
	AWeaponBase* GetWeaponByID(const int WeaponID) const;

	/** Returns the current weapon equipped by the player */
	UFUNCTION(BlueprintCallable, Category = "Inventory Component")
	AWeaponBase* GetCurrentWeapon() const {return CurrentWeapon; }

	/**  Returns the amount of ammunition currently loaded into the weapon */
	UFUNCTION(BlueprintCallable, Category = "Inventory Component")
	FText GetCurrentWeaponLoadedAmmo() const
	{
		if (CurrentWeapon != nullptr)
		{
			return FText::AsNumber(CurrentWeapon->GetRuntimeWeaponData()->ClipSize);
		}
		UE_LOG(LogProfilingDebugging, Log, TEXT("Cannot find Current Weapon"));
		return FText::FromString("0");
	} 

	/** Returns the amount of ammunition remaining for the current weapon */
	UFUNCTION(BlueprintCallable, Category = "Inventory Component")
	FText GetCurrentWeaponRemainingAmmo() const;

	int32 GetCurrentWeaponCarriedAmmo() const;

	UFUNCTION(BlueprintCallable, Category = "Inventory Component")
	FName GetCurrentWeaponDisplayName() const
	{
		if (CurrentWeapon != nullptr)
		{
			return CurrentWeapon->GetStaticWeaponData()->WeaponName;
		}
		UE_LOG(LogProfilingDebugging, Log, TEXT("Cannot find Current Weapon"));
		return TEXT("Currentweapon is nullptr!");
	}

	UFUNCTION(BlueprintCallable, Category = "Inventory Component")
	UTexture2D* GetCurrentWeaponDisplayImage() const
	{
		if (CurrentWeapon != nullptr)
		{
			return CurrentWeapon->GetStaticWeaponData()->WeaponIcon;
		}
		UE_LOG(LogProfilingDebugging, Log, TEXT("Cannot find Current Weapon"));
		return nullptr;
	}
	
	UPROPERTY(BlueprintAssignable, Category = "Inventory Component")
	FHitActor EventHitActor;

	UPROPERTY(BlueprintAssignable, Category = "Inventory Component")
	FFailedToReload EventFailedToReload;

	/** The input actions implemented by this component */
	UPROPERTY()
	UInputAction* FiringAction;

	UPROPERTY()
	UInputAction* PrimaryWeaponAction;

	UPROPERTY()
	UInputAction* SecondaryWeaponAction;

	UPROPERTY()
	UInputAction* ReloadAction;
	
	UPROPERTY()
	UInputAction* ScrollAction;

	UPROPERTY()
	UInputAction* InspectWeaponAction;

private:

	/** Spawns starter weapons */
	virtual void BeginPlay() override;

	/** Swap to a new weapon
	 *	@param SlotId The ID of the slot which to swap to
	 */

	void PlaySwapAnimation();
	void SwapWeapon(int SlotId);

	void AttachWeaponBack(AWeaponBase* InWeapon);
	void AttackWeaponHand(AWeaponBase* InWeapon);

	void CompleteWeaponSwap(int SlotId);

	UFUNCTION(Server,Reliable)
	void ServerSwapWeapon(int SlotId);


	/** Swaps to the weapon in CurrentWeaponSlot */

	/**	Template function for SwapWeapon (used with the enhanced input component) */
	template <int SlotID>
	void SwapWeapon() { SwapWeapon(SlotID); }
	
	/** Swaps between weapons using the scroll wheel */
	void ScrollWeapon(const FInputActionValue& Value);


	bool CanFire();

	bool CanReload();

	/** Fires the weapon */
	void StartFire();

	UFUNCTION(Server,Reliable)
	void ServerFire();

	UFUNCTION(NetMulticast,Reliable)
	void MulticastFire(int32 RandomSeed);

	/** Stops firing the weapon */
	void StopFire();

	UFUNCTION(Server, Reliable)
	void ServerStopFire();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStopFire();

	/** Reloads the weapon */
	void Reload();

	UFUNCTION(Server, Reliable)
	void ServerReload();

	void UpdateWeaponAmmo();

	void UpdateCarriedAmmo();

	void HandleReloading();

	/** Plays an inspect animation on the weapon */
	void Inspect();

	void HandleUnequip();

	void UnequipReturn();

	void StartFireTimer();

	void FinishFireTimer();

	void StopFireTimer();

	void ReadyToFire();

	void PlayReloadAnimation();
	
	UFUNCTION()
	void OnRep_CurrentWeapon(AWeaponBase* PrevCurrentWeapon);

	UFUNCTION()
	void OnRep_PerformingWeaponSwap();

	UFUNCTION()
	void OnRep_CharacterState();

	UFUNCTION()
	void OnRep_CarriedAmmo();

	/** Whether to print debug statements to the screen */
	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	bool bDrawDebug = false;

	/** The distance at which pickups for old weapons spawn during a weapon swap */
	UPROPERTY(EditDefaultsOnly, Category = "Camera | Interaction")
	float WeaponSpawnDistance = 100.0f;

	/** THe Number of slots for Weapons that this player has */
	UPROPERTY(EditDefaultsOnly, Category = "Weapons | Inventory")
	int NumberOfWeaponSlots = 2;

	/** An array of starter weapons. Only weapons within the range of NumberOfWeaponSlots will be spawned */
	UPROPERTY(EditDefaultsOnly, Category = "Weapons | Inventory")
	TArray<FStarterWeaponData> StarterWeapons;

	UPROPERTY(EditDefaultsOnly, Category = "Weapons | Behaviour")
	EReloadFailedBehaviour ReloadFailedBehaviour = EReloadFailedBehaviour::Ignore;

	UPROPERTY(ReplicatedUsing = OnRep_CharacterState)
	ECharacterState CharacterState = ECharacterState::ECS_Unoccupied;

	/** How many times we should retry the reload before cancelling it. Set to 0 for unlimited. */
	UPROPERTY(EditDefaultsOnly, Category = "Weapons | Behaviour")
	int MaxRetryAmount = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Weapons | Behaviour")
	float RetryInterval = 0.1f;

	/** The amount of times we have re-tried the reload animation */
	int RetryAmount; 

	UPROPERTY(EditDefaultsOnly, Category = "Weapons | Behaviour")
	EMovementState TargetMovementState;

	UPROPERTY(EditDefaultsOnly, Category = "Weapons | Behaviour")
	EWeaponSwapBehaviour WeaponSwapBehaviour = EWeaponSwapBehaviour::UseNewValue;
	
	/** The integer that keeps track of which weapon slot ID is currently active */
	UPROPERTY(Replicated)
	int CurrentWeaponSlot;

	/** The integer that keeps track of which weapon slot ID we are aiming to switch to while waiting for the unequip animation to play */
	UPROPERTY(Replicated)
	int TargetWeaponSlot;

	UPROPERTY(ReplicatedUsing = OnRep_PerformingWeaponSwap)
	bool bPerformingWeaponSwap;

	bool bIsWeaponReadyToFire = true;

	UPROPERTY(ReplicatedUsing = OnRep_CarriedAmmo)
	int32 CarriedAmmo;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TMap<EAmmoType, int32> AmmoMap;

	/** A Map storing the player's current weapons and the slot that they correspond to */
	//UPROPERTY(VisibleAnywhere, Category = "Equipment")
	//TMap<int, AWeaponBase*> EquippedWeapons;

	UPROPERTY(Replicated, VisibleAnywhere, Category = "Equipment")
	AWeaponBase* PrimaryWeapon;

	UPROPERTY(Replicated, VisibleAnywhere, Category = "Equipment")
	AWeaponBase* SecondaryWeapon;

	/** The player's currently equipped weapon */
	UPROPERTY(ReplicatedUsing = OnRep_CurrentWeapon, VisibleAnywhere)
	AWeaponBase* CurrentWeapon;

	FTimerHandle ReloadRetry;

	FTimerHandle WeaponSwapDelegate;

	FTimerHandle ShotDelay;

	FTimerHandle SpamFirePreventionDelay;



};
